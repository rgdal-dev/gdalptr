
<!-- README.md is generated from README.Rmd. Please edit that file -->

# gdalptr

<!-- badges: start -->
<!-- badges: end -->

The goal of gdalptr is to learn about raw pointer handling and interface
building from [adbcdrivermanager](https://github.com/paleolimbot/radbc/)
and other fab projects by Dewey Dunnington.

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

(there was some work with the Rcpp wrapper left in branch ‘rcpp-0.0.1’).

We can have some fun with this, the dsn package has a few good online
examples.

``` r
## remotes::install_github("hypertidy/dsn")

GDALGetRasterSize(GDALOpen(dsn::gebco22()))
#> [1] 86400 43200

GDALGetRasterSize(GDALOpen(dsn::wms_arcgis_mapserver_tms()))
#> [1] 33554432 33554432
```

The recursive VSI listing will tell us what files live in a tarball, a
zip, a url endpoint, etc

``` r
tarball <-  pak::cache_list()$fullpath[1]
#> gdalptr dataset finalizer called to free the C pointer memory
#> gdalptr dataset finalizer called to free the C pointer memory
basename(tarball)
#> [1] "jsonlite_1.8.4.tar.gz"
fs <- GDALVSIReadDirRecursive(file.path("/vsitar", tarball))
str(fs)
#>  chr [1:197] "jsonlite/" "jsonlite/NAMESPACE" "jsonlite/LICENSE" ...
```

With an upcoming GDAL release (3.7), we can even target different
aspects via GDAL’s virtual driver, *without constructing actual VRT or
opening the dataset*.

``` r
## because I'm working with the dev source
GDALVersion()
#> [1] "GDAL 3.8.0dev-78e9c2fc49, released 2023/05/02"

## we can target an overview in this massive tile dataset
GDALGetRasterSize(GDALOpen(sprintf("vrt://%s?ovr=12", dsn::wms_arcgis_mapserver_tms())))
#> [1] 4096 4096


GDALGetRasterSize(GDALOpen(sprintf("vrt://%s?ovr=6", dsn::wms_arcgis_mapserver_tms())))
#> [1] 262144 262144

GDALGetRasterSize(GDALOpen(sprintf("vrt://%s?ovr=0", dsn::wms_arcgis_mapserver_tms())))
#> [1] 16777216 16777216
```
