## User code build options

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