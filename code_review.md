# Abi code review for Light

## Code Structure & CMake	Proper folders, clean build system:

There are some issues regarding importing header files. After updating the project structure, you forgot to update the paths to the files correctly which leads to many errors on a fresh run. You probably didn't run into any issues because it may have been building from cache on your local machine.

Regarding the comments made above, I think your `CMake` file takes care of this. For some reason when I first cloned the project, CLion was complaining about the file paths. I think it’s just an IDE difference.


Something I learned while looking through your code is the use of `stdio.h` as well as `windows.h` to ensure your code is runnable across platforms while using `popen`.

## Modularity & Readability	Clear function separation, descriptive names:

I like how you modularized your code. When I first looked at it I thought it was perhaps *too* modular. However, as I kept looking through I began to like how you broke your code up and separated each file to its own functionality. This will be much easier to maintain as the project grows.

Each file is very well named and it is clarifies the functionality that will be found inside the file. The variable naming is also very descriptive and easy to follow.

## Test Coverage	Missing logic? Weak edge cases? Redundant tests?

You do a good job of testing the core functionality of your program, especially the `process_input_original` and `process_input_target`. You were very thorough in testing all the different possibilities for what the user could input.

The only functionality you didn’t account for was the for the `convert_currency` and `get_exchange_rate` functions which is understandable as they require dependency injection and refactoring the existing code.

## Bugs or Gaps Identified	Any logic failures or unhandled input?

I did not find any logic gaps or unhandled inputs, but there are a few very small nitpicks and suggestions I have listed below.

One small nitpick I have is in the `get_exchange_rate.cpp` file: as far as my understanding, `malloc` is rarely used in C++ as we have more idiomatic ways to dynamically allocate memory in a safer way. On line 26, rather than static casting the buffer to type `char` we can instead directly allocate a vector of type char. This will also save us needing to remember to free the allocated memory, preventing memory leaks:

~~char* buf = static_cast<char*> (malloc(1024));~~ --> 
`vector<char> buf(1024);`

Then you can modify your code to use the underlying `char* arr`: ```c++ fgets(buf.data(), 1024, pipe)… ```

You also do a good check to ensure the `buf` exists before trying to access it and free the memory allocated to it once you are done.

You do a great job of handling and parsing user input with your `process_input` functions. Great job at covering negative price inputs, defaulting empty prices to 1, and printing helpful prompts to the user if they misuse the application.

Small nitpick. Rather than typing the string `"0123456789.-“` multiple times in the `process_input_original.cpp`, you could make it a constant variable in your `hpp` file. This is not an issue in this project, but in the case that the project grows and you need to process more arguments, it could be beneficial.

I also really like your `strip_and_to_lower` helper function to format the input.

For `currency_names` and `currency_descriptions`, instead of having two separate arrays holding the names and descriptions of each currency at a specific index, you could opt for a map to hold all of the information in one data structure. This would be easier to maintain in the long run and eliminate the need for two header files to store this information. The key could be the currency name with the value being the description.

## Test Output & Clarity	Are test names/output readable and meaningful?

The test names are clear and well formatted.


## 5 New Google Test Cases:

This project is already well tested with all the core functionalities accounted for. I created 2 new tests and refactored the project to test the difficult portions to test that use `curl` commands to fetch data.