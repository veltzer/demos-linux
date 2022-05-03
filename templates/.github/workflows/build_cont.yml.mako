<%!
    import config.python
%>name: build
on: [push, pull_request, workflow_dispatch]
jobs:
  build:
    name: Build on ${"${{ matrix.container }}"}
    runs-on: ubuntu-latest
    container: ${"${{ matrix.container }}"}
    strategy:
      matrix:
        container: ${config.python.test_container}
        python-version: ${config.python.test_python}
    steps:
    - uses: actions/checkout@v3
    - name: Set up Python ${"${{ matrix.python-version }}"}
      uses: actions/setup-python@v3
      with:
        python-version: ${"${{ matrix.python-version }}"}
# This is needed for the python stuff to complie (stupid 20.10!)
    - name: Install libffi.so.7
      run: |
        apt install wget
        wget https://mirrors.edge.kernel.org/ubuntu/pool/main/libf/libffi/libffi7_3.3-4_amd64.deb
        dpkg --install libffi7_3.3-4_amd64.deb
    - name: Install python requirements
      run: python -m pip install -r requirements.txt
    - name: pydmt build_tools
      run: pydmt build_tools --sudo False
    - name: Build
      run: pymakehelper make DO_STP=0
