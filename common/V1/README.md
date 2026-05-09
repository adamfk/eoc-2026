Another typical C state machine implementation.

Pros/Cons:
- ✅ state variables are private (file static) in .c
- ✅`.c` file is NOT included into `.cpp` file for unit testing.
- ✅`.c` file is compiled as `C` (not `C++`) for unit testing.
- public setup method.
    - should only be called once at startup in production code
    - 👍easy to reset state for unit testing
- public `get_state()` function.
    - 👍useful for testing
    - available to production code as well because it is often useful for display/logging
- UNIT_TESTING accessors are provided for "Hand of God" unit testing.
    - works OK, but has some mild downsides.
    - 👎clutters up .h and .c files a bit.
    - 👎extra build step to restrict access to unit testing. Not strictly required, but can be helpful.

![fsm](fsm.png)
