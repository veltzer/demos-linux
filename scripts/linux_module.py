#!/usr/bin/env python

"""
Build the kernel modules of this repository as an rsconstruct mass generator.

    linux_module.py plan  SRC_ROOT OUT_DIR   # print the manifest of every .ko
    linux_module.py build SRC_ROOT OUT_DIR   # run kbuild for all of them

A linux-module.yaml sits in a module's source directory and names the modules
built there (the format rsconstruct's linux_module creator used to read):

    make: make                   # optional
    kdir: /lib/modules/X/build   # optional, default: the running kernel's
    arch: x86_64                 # optional, passed as ARCH=
    cross_compile: aarch64-linux-gnu-  # optional, passed as CROSS_COMPILE=
    v: 0                         # optional, V= verbosity
    w: 1                         # optional, W= warning level
    modules:
      - name: hello              # produces hello.ko
        sources: [main.c]        # relative to the yaml directory
        extra_cflags: [-DDEBUG]  # optional, CFLAGS_ of its objects

For every manifest the build writes one Kbuild naming all its modules next to
the yaml (one kbuild pass, so a module can use symbols another one exports), runs
`make -C KDIR M=<that directory> modules`, keeps the .ko files in memory, runs
`make ... clean`, removes the Kbuild again and writes the .ko to
OUT_DIR/<yaml directory>/<name>.ko. kbuild needs an absolute M= pointing at
the sources, which is why the build cannot happen inside OUT_DIR.
"""

import json
import os
import subprocess
import sys
from dataclasses import dataclass, field
from typing import NoReturn

import yaml

MANIFEST_NAME = "linux-module.yaml"


@dataclass
class Module:
    name: str
    sources: list[str]
    extra_cflags: list[str] = field(default_factory=list)


@dataclass
class Project:
    manifest: str
    make: str
    kdir: str | None
    arch: str | None
    cross_compile: str | None
    v: int
    w: int
    modules: list[Module]

    @property
    def directory(self) -> str:
        return os.path.dirname(self.manifest)


def fail(message: str) -> NoReturn:
    sys.stderr.write(f"{sys.argv[0]}: {message}\n")
    sys.exit(1)


def string_list(table: dict[str, object], key: str, where: str) -> list[str]:
    value = table.get(key, [])
    if not isinstance(value, list):
        fail(f"{where}: '{key}' must be a list of strings")
    strings: list[str] = []
    for item in value:
        if not isinstance(item, str):
            fail(f"{where}: '{key}' must be a list of strings")
        strings.append(item)
    return strings


def optional_string(table: dict[str, object], key: str, where: str) -> str | None:
    value = table.get(key)
    if value is None:
        return None
    if not isinstance(value, str):
        fail(f"{where}: '{key}' must be a string")
    return value


def parse_project(manifest: str) -> Project:
    with open(manifest, encoding="utf-8") as handle:
        raw = yaml.safe_load(handle)
    if not isinstance(raw, dict):
        fail(f"{manifest}: the manifest must be a mapping")
    modules: list[Module] = []
    for entry in raw.get("modules", []):
        if not isinstance(entry, dict):
            fail(f"{manifest}: every module must be a mapping")
        name = entry.get("name")
        if not isinstance(name, str) or not name:
            fail(f"{manifest}: a module needs a non-empty 'name'")
        sources = string_list(entry, "sources", f"{manifest} [{name}]")
        if not sources:
            fail(f"{manifest} [{name}]: 'sources' must not be empty")
        modules.append(
            Module(name, sources, string_list(entry, "extra_cflags", manifest))
        )
    if not modules:
        fail(f"{manifest}: 'modules' must name at least one module")
    return Project(
        manifest=manifest,
        make=str(raw.get("make", "make")),
        kdir=optional_string(raw, "kdir", manifest),
        arch=optional_string(raw, "arch", manifest),
        cross_compile=optional_string(raw, "cross_compile", manifest),
        v=int(raw.get("v", 0)),
        w=int(raw.get("w", 1)),
        modules=modules,
    )


def find_projects(src_root: str) -> list[Project]:
    manifests: list[str] = []
    for dirpath, dirnames, filenames in os.walk(src_root):
        dirnames.sort()
        if MANIFEST_NAME in filenames:
            manifests.append(os.path.join(dirpath, MANIFEST_NAME))
    return [parse_project(m) for m in sorted(manifests)]


def ko_path(project: Project, module: Module, out_dir: str) -> str:
    return os.path.join(out_dir, project.directory, f"{module.name}.ko")


def plan(projects: list[Project], out_dir: str) -> dict[str, object]:
    outputs = []
    for project in projects:
        for module in project.modules:
            sources = [project.manifest]
            sources.extend(
                os.path.normpath(os.path.join(project.directory, s))
                for s in module.sources
            )
            outputs.append(
                {"path": ko_path(project, module, out_dir), "sources": sources}
            )
    return {"version": 1, "outputs": outputs}


def running_kernel_build_dir() -> str:
    result = subprocess.run(
        ["uname", "-r"], check=False, capture_output=True, text=True
    )
    if result.returncode != 0:
        fail(
            "'uname -r' failed; set 'kdir' in the manifest to name the kernel build directory"
        )
    return f"/lib/modules/{result.stdout.strip()}/build"


def run(cmd: list[str], cwd: str) -> None:
    sys.stderr.write(" ".join(cmd) + "\n")
    result = subprocess.run(cmd, check=False, cwd=cwd)
    if result.returncode != 0:
        fail(f"command failed with exit code {result.returncode}: {' '.join(cmd)}")


def kbuild_command(
    project: Project, kdir: str, module_dir: str, goal: str
) -> list[str]:
    cmd = [project.make, "-C", kdir]
    if project.arch:
        cmd.append(f"ARCH={project.arch}")
    if project.cross_compile:
        cmd.append(f"CROSS_COMPILE={project.cross_compile}")
    cmd.append(f"M={module_dir}")
    cmd.append(f"V={project.v}")
    if goal == "modules":
        cmd.append(f"W={project.w}")
    cmd.append(goal)
    return cmd


def kbuild_content(project: Project) -> str:
    lines = [f"obj-m := {' '.join(m.name + '.o' for m in project.modules)}"]
    for module in project.modules:
        objects = [os.path.splitext(s)[0] + ".o" for s in module.sources]
        lines.append(f"{module.name}-objs := {' '.join(objects)}")
        if module.extra_cflags:
            # Per object, since ccflags-y would reach every module of the directory.
            lines.extend(
                f"CFLAGS_{o} := {' '.join(module.extra_cflags)}" for o in objects
            )
    return "\n".join(lines) + "\n"


def build_project(project: Project, kdir: str, out_dir: str) -> None:
    module_dir = os.path.abspath(project.directory)
    kbuild = os.path.join(module_dir, "Kbuild")
    with open(kbuild, "w", encoding="utf-8") as handle:
        handle.write(kbuild_content(project))
    ko_bytes: dict[str, bytes] = {}
    try:
        run(kbuild_command(project, kdir, module_dir, "modules"), cwd=module_dir)
        for module in project.modules:
            ko_source = os.path.join(module_dir, f"{module.name}.ko")
            if not os.path.exists(ko_source):
                fail(f"make succeeded but produced no {ko_source}; check the module name")
            # Read the .ko before `make clean`, which deletes every .ko under M=.
            with open(ko_source, "rb") as handle:
                ko_bytes[module.name] = handle.read()
    finally:
        run(kbuild_command(project, kdir, module_dir, "clean"), cwd=module_dir)
        os.remove(kbuild)
    for module in project.modules:
        destination = ko_path(project, module, out_dir)
        os.makedirs(os.path.dirname(destination), exist_ok=True)
        with open(destination, "wb") as handle:
            handle.write(ko_bytes[module.name])


def main() -> None:
    if len(sys.argv) != 4 or sys.argv[1] not in ("plan", "build"):
        fail("usage: linux_module.py plan|build SRC_ROOT OUT_DIR")
    mode, src_root, out_dir = sys.argv[1:]
    projects = find_projects(src_root)
    if mode == "plan":
        json.dump(plan(projects, out_dir), sys.stdout, indent=2)
        sys.stdout.write("\n")
        return
    for project in projects:
        kdir = project.kdir or running_kernel_build_dir()
        build_project(project, kdir, out_dir)


if __name__ == "__main__":
    main()
