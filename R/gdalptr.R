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
GDALOpen<- function(dsn = NULL) {
  if (is.null(dsn)) dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
  if (file.exists(dsn)) dsn <- normalizePath(dsn)
  if (!nzchar(dsn[1]) || is.na(dsn[1])) stop("invalid dsn")
  ds <- GDALDataset()
  
    .Call("GdalPtrOpen", ds, dsn[1])
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

#' Get the GDAL version string
#' 
#' @return character vector describing GDAL version
#' @export
#'
#' @examples
#' GDALVersion()
GDALVersion <- function() {
  .Call("GdalVersion")
}

new_env <- function() {
  new.env(parent = emptyenv())
}
