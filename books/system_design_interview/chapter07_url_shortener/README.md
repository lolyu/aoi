# URL shortener
* HTTP redirect:
    * 301 redirect
        * Status 301 means that the resource (page) is moved permanently to a new location. The client/browser should not attempt to request the original location but use the new location from now on
        * permanent redirecting
    * 302 redirect
        * Status 302 means that the resource is temporarily located somewhere else, and the client/browser should continue requesting the original url.
        * temporary redirecting

* two approaches:
    * hash function + collision resolution
    * ID generator + base62 conversion


## hash function + collision resolution

<img width="662" alt="image" src="https://github.com/lolyu/aoi/assets/35479537/72fe6d29-f24f-4acd-8d8b-2011b45c2291">

* to resolve collision, recursively append predefined string until no more collision is discovered
* to find if a short URL exists, use bloom filter

## ID generator + base62 conversion

<img width="579" alt="image" src="https://github.com/lolyu/aoi/assets/35479537/ff3d18ed-78ff-49cf-a776-a92ac3805ef5">
