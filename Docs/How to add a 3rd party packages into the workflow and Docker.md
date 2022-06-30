1.  [How to add packages into the workflow](#how-to-add-packages-into-the-workflow)
2.  [How to add packages into Docker image](#how-to-add-packages-into-docker-image)

# Workflow and GitHub Actions.

A workflow is a configurable automated process made up of one or more jobs. You must create a YAML file to define your workflow configuration.
**If you might need it!**

## About YAML syntax for workflows
Workflow files use YAML syntax, and must have either a .yml or .yaml file extension. If you're new to YAML and want to learn more, see ["Learn YAML in five minutes."](https://www.codeproject.com/Articles/1214409/Learn-YAML-in-five-minutes)

In our repository we store workflow files in the .github/workflows directory. (build.yml)
***

Here is an example of how to write such a file and how to include the packages we need: 

* The name of your workflow. GitHub displays the names of your workflows on your repository's actions page. If you omit name, GitHub sets it to the workflow file path relative to the root of the repository.

```yaml
name: Clang-Build
```
* Required. The name of the GitHub event that triggers the workflow. You can provide a single event string, array of events, array of event types, or an event configuration map that schedules a workflow or restricts the execution of a workflow to specific files, tags, or branch changes. For a list of available events, see ["Events that trigger workflows."](https://docs.github.com/en/articles/events-that-trigger-workflows)

Example using a single event:
```yaml
# Triggered when code is pushed to any branch in a repository
on: push
```
Example using a list of events
```yaml
# Triggers the workflow on push or pull request events
on: [push, pull_request]
```
Another example
```yaml
# Controls when the action will run. Triggers the workflow on push or pull request
# events but only for the master branch
on:
  push:
    branches: [ master,dev/develop,dev/coroutine,dev/clang_build]
  pull_request:
    branches: [ master,dev/develop,dev/coroutine]
```

* A workflow run is made up of one or more jobs. Jobs run in parallel by default. To run jobs sequentially, you can define dependencies on other jobs using the jobs.<job_id>.needs keyword.

Each job runs in a runner environment specified by runs-on.

You can run an unlimited number of jobs as long as you are within the workflow usage limits. 

Each job must have an id to associate with the job. The key job_id is a string and its value is a map of the job's configuration data. You must replace <job_id> with a string that is unique to the jobs object. The <job_id> must start with a letter or _ and contain only alphanumeric characters, -, or _.

Example
```yaml
jobs:
  my_first_job:
    name: My first job
  my_second_job:
    name: My second job
jobs.<job_id>.name
```
The name of the job displayed on GitHub.

jobs.<job_id>.needs
Identifies any jobs that must complete successfully before this job will run. It can be a string or array of strings. If a job fails, all jobs that need it are skipped unless the jobs use a conditional expression that causes the job to continue.

```yaml
Example requiring dependent jobs to be successful
jobs:
  job1:
  job2:
    needs: job1
  job3:
    needs: [job1, job2]
```

In this example, job1 must complete successfully before job2 begins, and job3 waits for both job1 and job2 to complete.
The jobs in this example run sequentially: job1, job2, job3.

* GitHub-hosted runners
If you use a GitHub-hosted runner, each job runs in a fresh instance of a virtual environment specified by runs-on.

Available GitHub-hosted runner types are: \
Windows Server 2019 \
Ubuntu 20.04 \
Ubuntu 18.04 \
Ubuntu 16.04 \
macOS Big Sur 11.0 \
macOS Catalina 10.15 

Example
```yaml
runs-on: ubuntu-latest
```

**Self-hosted runners**
To specify a self-hosted runner for your job, configure runs-on in your workflow file with self-hosted runner labels.

All self-hosted runners have the self-hosted label, and you can select any self-hosted runner by providing only the self-hosted label. Alternatively, you can use self-hosted in an array with additional labels, such as labels for a specific operating system or system architecture, to select only the runner types you specify.

Example
```yaml
runs-on: [self-hosted, linux]
```

* Workflow dispatch Allows you to run this workflow manually from the Actions tab.
Example
```yaml
  workflow_dispatch:
```
Result - we can click to button 'Run workflow'

![](https://i2.wp.com/user-images.githubusercontent.com/1865328/86147571-2de93700-babf-11ea-8a08-e4beffd3abe9.png?ssl=1)

*  Steps represent a sequence of tasks that will be executed as part of the job.
1. All config is written in yaml, so it is important to follow the syntax and indentation, a new entity in the list starts with "-" 
2. The steps are essentially an ordered list. They will be executed one by one strictly in sequence 

Example
```yaml
  steps:
```

* Name. Name is just for display in the interface, you can do without it just fine.

Example
```yaml
  name: build
```

*  Uses. Specify an action to run as part of a step in your job. You can use an action defined in the same repository as the workflow, a public repository elsewhere on GitHub, or in a published Docker container image. Here we specify the name of some already written action, if we want to use it. The action can be a specific branch in a specific repository (any repository), it can be code stored in a neighboring folder, or it can be a docker-image ([full list](https://docs.github.com/en/actions/reference/workflow-syntax-for-github-actions#jobsjob_idstepsuses))
This example uses ```actions/checkout@v2```. You can easily find them [by name ](https://github.com/marketplace/actions/checkout) in the marketplace and look at the source code of a specific version (it comes after the @ in the name) to see what they do. checkout does a pull of the repository and the branch in which it is running.

tag is strongly recommended:

checks-out your repository under $GITHUB_WORKSPACE, so your job can access it
```yaml
uses: actions/checkout@v2
```
for actions in a public repository
```yaml
uses: {owner}/{repo}@{ref} 
```
for actions in a subdirectory of a public repository
```yaml
uses: {owner}/{repo}/{path}@{ref} 
```
for actions in a a subdirectory of the same repository
```yaml
uses: ./path/to/dir 
```
for actions on Docker Hub
```yaml
uses: docker://{image}:{tag} 
```
for actions in a public registry
```yaml
uses: docker://{host}/{image}:{tag} 
```

* Run. Runs some command in the shell. You can't use a shell command together with an action, they must live in different steps

```yaml
run: |
  sudo docker build . -t juniorgram:1.0
  docker run --rm --name juniorgram-container juniorgram:1.0
```

## How to add packages into the workflow
* Let's move on to the most important thing, how do we use the packages that we need. It really depends on the environment in which you are running virtual machine on Github or locally.

If you want to use the packages needed to implement a new feature, it is recommended to first find out **if they are included in the standard build of the machine** on which you want to test your feature. Take, for example, ubuntu.We need cmake for the build but we have it in the standard build, then we know that ubuntu has the apt package manager and we can use it to install our cmake. 

Example 

```yaml
   - name: Setup CMake
      run: |
        sudo apt-get -y install cmake
        cmake --version 
```
We also do sudo ```sudo apt update``` to get the latest version of the packages we need. 
The following example shows how we can use a package that is originally in the ubuntu distribution:

```yaml
      - name: Build Docker image.
        run: |
          sudo docker build . -t juniorgram:1.0
          docker run --rm --name juniorgram-container juniorgram:1.0
```

The following examples show you how to install certain packages:

```yaml 
- name: Setup CLang compiler
      run: |
        sudo apt-get update
        sudo apt-get -y update
        sudo bash -c "$(wget -O - https://apt.llvm.org/llvm.sh)"
    - name: Setup CMake
      run: |
        sudo apt-get -y install cmake
        cmake --version
    - name: Setup python
      uses: actions/setup-python@v2
      with:
        python-version: '3.8'

    - name: Install pip dependencies
      uses:  BSFishy/pip-action@v1
      with:
        packages: |
          conan
          mako
    - name: Setup conan dependencies
      run: |
        conan remote add bincrafters https://api.bintray.com/conan/bincrafters/public-conan
    - name: Download libraries from Conan
      run: |
        export CC=/usr/bin/clang
        export CXX=/usr/bin/clang++
        mkdir ${{ github.workspace }}/Firmware/build && cd ${{ github.workspace }}/Firmware/build
        conan install ${{ github.workspace }}/Firmware/conanfile.txt --install-folder ${{ runner.workspace }}/Firmware/build/  -s compiler=clang -s compiler.version=10 --build=missing
```
# How to add packages into Docker image

Below is an example of our docker file and how it looks like to install the necessary packages. You can use this file to make your own docker file with the packages you need.

Our Dockerfile: 
```dockerfile
FROM alpine:latest - specifies the base (parent) image.

COPY . /usr/src/build - copies the contents of the current directory to /usr/src/build.

WORKDIR /usr/src/build - sets current directory as working directory. All commands will be built from this directory.

RUN apk update && apk upgrade && apk add --no-cache cmake make gcc g++ bash git python3 linux-headers - his line specifies the packages you need to run. They are listed with a space after the --no-cache flag. 

RUN wget -qO- https://bootstrap.pypa.io/get-pip.py | python3 \
&& pip3 install conan - in this line we use wget to download the file and give the output from the downloaded file to the python3 interpreter, pip3 is installed and conan is installed to install the libraries. 

RUN cmake -DCMAKE_BUILD_TYPE=Release . \
&& cmake --build . - run build project
```
Again, since we are using the alpine distribution, before installing the necessary packages, it is worth looking to see if they are in the apk package manager([packages list ](https://pkgs.alpinelinux.org/packages)). If they are there, you can install them as shown in the example. If not, then it makes sense to see if these packages are in pip. If not, then there is an option to build from source files :) 
