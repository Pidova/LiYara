
# LiYara
Fast, lightweight, and modular display library for programming and displaying LED modules.

## Table of Contents
- [Getting started](#Getting-started)
- [Contributing](#Contributing)
- [Library Documentation](LiYara/docs/DOCUMENTATION.md)

## Getting-started
### Structure
The display is segmented out meaning you have to make an object of one segment and add it to the display. On the bytecode level you can interpret it as a regular display this structure allows you to easily program it and easily add new segments in the future. 

### Prerequisites
List of libraries used for the LED drivers can be found [here](Liyara/docs/DOCUMENTATION.md).
C++23 standard is the recommended C++ standard.

### Code example
Here is a snippet of code with a 1 by 144 for the WS2812(Neopixels) to help you get started with initializing the display
```
    
    /* Initialize a module length of 1 with a hiegth of 144, starting color being purple with a brightness of 15% (Brightness is measured as a unsigned byte) */
    const auto module_1 = board::display::handler::ws2812::make_module_array(0u, 144u, board::display::colors::PURPLE, 40);

    /* Initialize a WS2812 with our total count being 144 and our data pin for that line being our config CONFIG_LED_DATALINE */
    const auto driver = board::display::handler::ws2812::get_driver(144u, CONFIG_LED_DATALINE); 

    /* Construct our display handler plus driver handler */
    board::display::displayer d;
    board::display::handler::ws2812::module m(module_1, driver); 

    /* Add that module at (0,0) */
    d.add(0u, 0u, m);
```
To get started with the VM you need to initialize a state and call the run function on it. VM reads can be hooked and behavoir can be edited on runtime exact details are documented in vm_state.
```

    /* Construct our state */
    board::animate::interpreter::vm_state state;

    /* Run our VM with our state and display */
    board::animate::interpreter::run(d, state);

```

## Contributing
Contributions for adding further support to other MCUs or LED drivers or bug fixes are welcomed. Adding new features are also welcomed though further support are treated with priority.