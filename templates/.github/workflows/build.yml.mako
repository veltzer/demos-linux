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
    - name: Install OS packages
      run: python -m scripts.install
    - name: Install python requirements
      run: python -m pip install -r requirements.txt
    - name: pydmt build_tools
      run: pydmt build_tools
    - name: Build
      run: make DO_STP=0
