# init SAI API

```cpp
    // sonic-sairedis/syncd/syncd_main.cpp
    auto vendorSai = std::make_shared<VendorSai>();
    auto syncd = std::make_shared<Syncd>(vendorSai, commandLineOptions, isWarmStart);
    syncd->run();
```

##
