
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
GDALGetInfo(xptr)  ## this should tell us the number of columns (1)
#> [1] 0
```

when we use Rcpp XPtr it works

``` r
xptr_ds <- xptr_GDALOpen(dsn)
#> 0
xptr_GDALGetRasterSize(xptr_ds)
#> [1] 20 15
```
