import argparse, getpass, os, re, sys, subprocess
from subprocess import CompletedProcess
from typing import Union, Any

# arguments
parser = argparse.ArgumentParser(description="Compile and Install Hyperpan using CMake")
parser.add_argument("-JUCE_INSTALL_PATH", type=str)
args = parser.parse_args()
# Var defs
envVars = os.environ.copy()
hyperpanDir = os.path.abspath(os.getcwd())
osname = sys.platform
username = getpass.getuser()
JUCEInstallFlag = args.JUCE_INSTALL_PATH


# Function defs
def checkforshellerr(returncode, stderr):
    # my way of getting around having to write this same line 20 times
    if returncode != 0:
        sys.exit(stderr)


# Verify installs
print(f'os: {osname}')

# CMake Verification
print("Verifying CMake installation")

cmakeVersion: Union[CompletedProcess[Any], CompletedProcess[str]] = subprocess.run(["cmake", "-version"], capture_output=True, text=True)

checkforshellerr(cmakeVersion.returncode, cmakeVersion.stderr)

# CMake Version verification
cmakeVersionNum = re.search(r'[0-9]\.\d+', cmakeVersion.stdout).group()
cmakeMajorVersionNum = int(cmakeVersionNum[0])
cmakeMinorVersionNum = int(cmakeVersionNum[2:])
if cmakeMajorVersionNum < 3:
    sys.exit(f'Error: CMake must be version 3.15 or newer. Installed CMake version is {cmakeVersionNum}')
if cmakeMajorVersionNum == 3 and cmakeMinorVersionNum < 15:
    sys.exit(f'Error: CMake must be version 3.15 or newer. Installed CMake version is {cmakeVersionNum}')

print(f'CMake version {cmakeVersionNum} > version 3.15')

# Verify working generator (Unix/Visual Studio)
print("Verifying CMake generator")

if osname == "darwin" or osname.startswith("linux"):
    envVars["CMAKE_GENERATOR"] = "Unix Makefiles"
    generator = "Unix Makefiles"
elif osname == "win32":
    # only worked with vs 16 for this, we will need to try more vs versions eventually
    envVars["CMAKE_GENERATOR"] = "Visual Studio 16 2019"
    generator = "Visual Studio 16 2019"
else:
    sys.exit("Error: Hipster detected")

cmakeHelp = subprocess.run(["cmake", "--help"], capture_output=True, text=True, env=envVars)
checkforshellerr(cmakeHelp.returncode, cmakeHelp.stderr)

if osname == "darwin" or osname.startswith("linux"):
    if "* Unix Makefiles" not in cmakeHelp.stdout:
        sys.exit("Error: Unix Makefiles Generator not functional")
if osname == "win32" and "* Visual Studio 16 2019" not in cmakeHelp.stdout:
    sys.exit(
        "Error: Visual Studio 16 2019 Generator not functional, visit https://visualstudio.microsoft.com/vs/features/cplusplus/ to download")

print(f'Generator: {generator}')

# JUCE install default path
if osname == "win32":
    JUCEinstallpath = f"/{username}/JUCE"
if osname == "darwin":
    JUCEinstallpath = f"/Users/{username}/Applications/JUCE"
if osname.startswith("linux"):
    JUCEinstallpath = f"/home/{username}/JUCE"

# set install path if flag is set
if JUCEInstallFlag is not None:
    JUCEinstallpath = JUCEInstallFlag

# TODO: just install it automatically at the path provided
if not os.path.isdir(JUCEinstallpath):
    sys.exit(
        f"Error: JUCE not installed at path {JUCEinstallpath}. Please see README.md for how to install JUCE\nYou can "
        f"set a custom install path with the -JUCE_INSTALL_PATH flag")

envVars["JUCE_INSTALL_PATH"] = JUCEinstallpath

# cmake commands
print(f"Building Hyperpan using {generator}")
cmakeBuild = subprocess.run(
    ["cmake", "-B", "cmake-build", "-DCMAKE_BUILD_TYPE=release", f"-DCMAKE_INSTALL_PREFIX={JUCEinstallpath}"],
    env=envVars)
checkforshellerr(cmakeBuild.returncode, cmakeBuild.stderr)

print("Compiling build scripts")
cmakeCompile = subprocess.run(["cmake", "--build", "cmake-build"], env=envVars)
checkforshellerr(cmakeCompile.returncode, cmakeCompile.stderr)

print("Done.")
