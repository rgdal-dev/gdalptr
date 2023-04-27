
<!-- README.md is generated from README.Rmd. Please edit that file -->

# gdalptr

<!-- badges: start -->
<!-- badges: end -->

The goal of gdalptr is to learn about raw pointer handling and interface
building from [radbc](https://github.com/paleolimbot/radbc/).

## Example that doesn’t work

This is a basic example doesnt work:

``` r
library(gdalptr)
dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
xptr <- GDALOpen(dsn)
#> Raster count: 1
GDALGetInfo(xptr)  ## this should tell us the raster count (1)
#> Raster version info: GDAL 3.7.0dev-7cd1a9bb31, released 2023/04/17 (debug build)
#> [1] 0
```
