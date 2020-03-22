## User code build options

20200323: PLEASE IGNORE THE CONTENT HERE, NOT UP TO DATE !

When compiling your code, you can add some compile-time options.
These come through symbols, that you can define (or not) before the "include" line:

```
#define HTTAG_SOME_OPTION
#include <cpphtmltags.hpp>
```

To avoid naming collisions, all of these (and the internal ones) are prefixed with `HTTAG_`.

- `HTTAG_SILENT_MODE`:

- `HTTAG_SILENT_WARNINGS`:

- `HTTAG_SILENT_ERRORS`:

- `HTTAG_NO_CHECK`: