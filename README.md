### This program may cause system failure(?) and comes with no warranty

# A demo program to write to address 0x0 on Linux
**Should succeed if ran as root, should fail if run as non-root user**

This is a demo program inspired by [Jacob Sorber's video](https://youtu.be/j38NsAsnH_s).\
The method to write to null is provided in a [comment by valverde](https://www.youtube.com/watch?v=j38NsAsnH_s&lc=Ugzk7FvseBh2iW30qgl4AaABAg).

The short of valverde's comment (supported by this program) is that you can write to a null pointer by
forcing mmap to not take the supplied address as a hint using the flag `MAP_FIXED` and running the program as root.
Otherwise, the write should fail and the program follows the value at `/proc/sys/vm/mmap_min_addr`.

It would be possible to change the value of `mmap_min_addr` for non-root users to be able to do so, but this is a bad idea.
To learn why (and why mmap_min_addr exists), read up on Null Pointer Dereference (NPD) attacks.
