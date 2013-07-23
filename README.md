# NewRelicShim

This is a .NET profiler that I wrote to inspect the behavior of another .NET profiler. If you want to use it with IIS, build it, register the COM object, and then set the value of the registry key `HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\services\W3SVC\Environment` to

```
COR_ENABLE_PROFILING=1
COR_PROFILER={039AA112-7D12-4089-AB1C-01C4CBEAF237}
```
