#!/usr/bin/env python

"""
This script wraps the running of the compiler with the right flags.
You must pass the compiler and the flags to it.
It will scan the source file in order to find specific flags
to be added to the compilation or linkage.
"""

import sys
import subprocess


def system_check_output(cmd):
    with subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as pipe:
        (out_stdout,out_stderr)=pipe.communicate()
        status=pipe.returncode
        if status:
            #raise ValueError(f"error in executing [{cmd}]")
            print(f"could not run [{cmd}]", file=sys.stderr)
            print("error follows...", file=sys.stderr)
            print(out_stderr.decode(), file=sys.stderr)
            print("output follows...", file=sys.stderr)
            print(out_stdout.decode(), file=sys.stderr)
            sys.exit(status)
        return out_stdout.decode()


def check_no_output(args):
    """ Run a command and check that it has not output """
    with subprocess.Popen(args, stdout=subprocess.PIPE, stderr=subprocess.PIPE) as pipe:
        (out_stdout,out_stderr)=pipe.communicate()
        status=pipe.returncode
        out_stdout = out_stdout.decode()
        out_stderr = out_stderr.decode()
        if status or out_stdout!="" or out_stderr!="":
            #raise ValueError(f"error in executing [{args}]")
            print(f"could not run [{args}]", file=sys.stderr)
            print("error follows...", file=sys.stderr)
            print(out_stderr, file=sys.stderr)
            print("output follows...", file=sys.stderr)
            print(out_stdout, file=sys.stderr)
            sys.exit(1)


def handle(line, args, string, process, subs, inject=None):
    f=line.find(string)
    if f!=-1:
        f=line.find(string)+len(string)
        cmd=line[f:]
        cmd=cmd.format(**subs)
        cmd=cmd.split()
        if inject is not None:
            args[inject:inject] = cmd
            return
        if process:
            out=subprocess.check_output(cmd).decode()
            out=out.split()
            args.extend(out)
        else:
            args.extend(cmd)


def handle_first(line, args, string):
    f=line.find(string)
    if f!=-1:
        f=line.find(string)+len(string)
        args[0]=line[f:]


def main():
    script=sys.argv.pop(0)
    doDebug=int(sys.argv.pop(0))
    ccache=int(sys.argv.pop(0))
    link=int(sys.argv.pop(0))
    source=sys.argv.pop(0)
    target=sys.argv.pop(0)
    args=sys.argv

    # substitutions

    if doDebug:
        print(f"script is [{script}]")
        print(f"source is [{source}]")
        print(f"target is [{target}]")
    subs={}
    subs["arch"]=subprocess.check_output(["arch"]).decode().rstrip()
    subs["deb_build_arch"]=subprocess.check_output([
        "dpkg-architecture",
        "-qDEB_BUILD_ARCH",
    ]).decode().rstrip()
    subs["deb_build_gnu_cpu"]=subprocess.check_output([
        "dpkg-architecture",
        "-qDEB_BUILD_GNU_CPU",
    ]).decode().rstrip()
    subs["deb_build_multiarch"]=subprocess.check_output([
        "dpkg-architecture",
        "-qDEB_BUILD_MULTIARCH",
    ]).decode().rstrip()
    subs["source"]=source
    subs["target"]=target
    # this is for the note.GNU-stack warning
    # https://stackoverflow.com/questions/73435637/how-can-i-fix-usr-bin-ld-warning-trap-o-missing-note-gnu-stack-section-imp
    if source.endswith(".S"):
        args.extend([
            "-z",
            "noexecstack",
        ])
    # scan the source code
    with open(source, "r") as stream:
        for line in stream:
            line=line.strip()
            if link:
                handle(line, args, "EXTRA_LINK_CMDS=", True, subs)
                handle(line, args, "EXTRA_LINK_FLAGS=", False, subs)
                handle_first(line, args, "LINKER=")
            else:
                handle(line, args, "EXTRA_COMPILE_CMDS=", True, subs)
                handle(line, args, "EXTRA_COMPILE_FLAGS=", False, subs, inject=1)
                handle(line, args, "EXTRA_COMPILE_FLAGS_AFTER=", False, subs)
                handle_first(line, args, "COMPILER=")
    if ccache and not link:
        args.insert(0,"ccache")
    if doDebug:
        print(f"running [{args}]")
    check_no_output(args)


if __name__ == "__main__":
    main()
