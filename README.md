
<!-- README.md is generated from README.Rmd. Please edit that file -->

# gdalptr

<!-- badges: start -->
<!-- badges: end -->

The goal of gdalptr is to learn about raw pointer handling and interface
building from [radbc](https://github.com/paleolimbot/radbc/) and other
fab projects by Dewey Dunnington.

## Basic example

This is a basic example, first with R’s C API (note use of
R_SetExternalPtrAddr in the source).

``` r
library(gdalptr)
dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
xptr <- GDALOpen(dsn)
GDALGetRasterSize(xptr)  
#> [1] 20 15
```

or with Rcpp (which makes the pointer handling easier in the source).

``` r
xptr_ds <- xptr_GDALOpen(dsn)
#> 0
xptr_GDALGetRasterSize(xptr_ds)
#> [1] 20 15
```

We can have some fun with this, the dsn package has a few good online
examples.

``` r
## remotes::install_github("hypertidy/dsn")

GDALGetRasterSize(GDALOpen(dsn::gebco22()))
#> [1] 86400 43200

GDALGetRasterSize(GDALOpen(dsn::wms_arcgis_mapserver_tms()))
#> [1] 33554432 33554432
```

With an upcoming GDAL release (3.7), we can even target different
aspects via GDAL’s virtual driver, *without constructing actual VRT or
opening the dataset*.

``` r
## because I'm working with the dev source
GDALVersion()
#> [1] "GDAL 3.7.0dev-717dcc0eed, released 2023/04/22 (debug build)"

## we can target an overview in this massive tile dataset
GDALGetRasterSize(GDALOpen(sprintf("vrt://%s?ovr=12", dsn::wms_arcgis_mapserver_tms())))
#> [1] 4096 4096


GDALGetRasterSize(GDALOpen(sprintf("vrt://%s?ovr=6", dsn::wms_arcgis_mapserver_tms())))
#> [1] 262144 262144

GDALGetRasterSize(GDALOpen(sprintf("vrt://%s?ovr=0", dsn::wms_arcgis_mapserver_tms())))
#> [1] 16777216 16777216
```
