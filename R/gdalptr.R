#' get a pointer to the abstract GDALDataset (not much use, see gdalptr::GDALOpen)
GDALDataset <- function() {
  .Call("GdalPtrDataset")
}


#' Open a GDAL dataset from a dsn
#'
#' @param dsn character, description of a file path, url, etc. 
#'
#' @return an external pointer 
#' @export
#'
#' @examples
#' dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
#' ds <- GDALOpen(dsn)
#' GDALGetRasterSize(ds)
GDALOpen<- function(dsn = NULL, read_only = TRUE) {
  if (is.null(dsn)) dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
  if (file.exists(dsn)) dsn <- normalizePath(dsn)
  if (!nzchar(dsn[1]) || is.na(dsn[1])) stop("invalid dsn")
  ds <- GDALDataset()
  
    .Call("GdalPtrGDALOpen", ds, dsn[1], read_only[1L])
}


#' Get the number of columns of  data set opened with gdalptr::GDALOpen()
#'
#' @param x external pointer returned by gdalptr::GDALOpen()
#'
#' @return integer vector ncol,nrow
#' @export
#'
#' @examples
#' dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
#' ds <- GDALOpen(dsn)
#' GDALGetRasterSize(ds)
GDALGetRasterSize <- function(x) {
  .Call("GdalPtrGetRasterSize", x)
}


GDALRasterIO <- function(x, window, data = NULL, resample_algo = "nearestneighbour") {
  .Call("GdalPtrRasterIO", x, as.integer(window), data, resample_algo)
}
#' Get the GDAL version string
#' 
#' @return character vector describing GDAL version
#' @export
#'
#' @examples
#' GDALVersion()
GDALVersion <- function() {
  .Call("GdalPtrGDALVersionInfo")
}


#' List VSI recursively 
#' 
#' I.e. get the file list from a zip file, so we can construct a dsn. 
#'
#' @param vsiname character string to GDAL source
#'
#' @return character vector of source contents
#' @export
#'
#' @examples
#' url <- file.path("/vsitar//vsicurl", 
#'        "https://cran.r-project.org/src/contrib/Archive/pixmap/pixmap_0.4-11.tar.gz")
#' if (interactive()) {
#'   files <- try(GDALVSIReadDirRecursive(url))
#'   if (!inherits(files, "try-error")) {
#'   dsn <- file.path(url, grep("logo.ppm", files, value = TRUE))
#'    ppm <- GDALOpen(dsn)
#'    GDALGetRasterSize(ppm) 
#'    
#'    }
#'   }
GDALVSIReadDirRecursive <- function(vsiname) {
  .Call("GdalPtrVSIReadDirRecursive", vsiname)
}
new_env <- function() {
  new.env(parent = emptyenv())
}
