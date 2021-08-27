# Hyperpan

Hyperpan is an audio plugin built on the [JUCE framework](https://github.com/juce-framework/JUCE) that pans a stereo signal given the mono sum amplitude of the signal.

## Installation

Hyperpan uses the [JUCE 6 CMake API](https://github.com/juce-framework/JUCE/blob/master/docs/CMake%20API.md) for compilation. If you would like to use Hyperpan at this moment, you will have to compile the plugin yourself (there are plans to release precompiled plugins).

### Dependencies:
* [The JUCE framework](https://juce.com/get-juce/download) (place the downloaded folder somewhere you will remember)
* [CMake](https://cmake.org/download/)
* [Visual Studio](https://visualstudio.microsoft.com/vs/features/cplusplus/) (This step is only needed for Windows users)

### Preparation
First, either clone the repository using git 

```git clone https://github.com/SeanMcGoff/Hyperpan.git```

or [download as a zip here](https://github.com/SeanMcGoff/Hyperpan/archive/refs/heads/main.zip) and extract the folder.

If you use windows, verify that CMake is using visual studio as your CMake generator by typing ```cmake -G``` and seeing if the asterisk is before a **Visual Studio** generator.

### Building
Locate where you placed the JUCE folder and open run these commands, replacing {Juce folder path} with the literal path to your JUCE folder:
```
    cmake -B cmake-build-install -DCMAKE_INSTALL_PREFIX={Juce folder path}/install
    cmake --build cmake-build-install --target install
```

Next, locate the Hyperpan folder and run these commands:
```
cmake -B cmake-build -DCMAKE_PREFIX_PATH={Juce folder path}/install
cmake --build cmake-build
```

The vst3 should be in the resulting *cmake-build* folder

