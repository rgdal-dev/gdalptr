#' get a pointer to the abstract GDALDataset (not much use, see gdalptr::GDALOpen)
GDALDataset <- function() {
  .Call("GdalPtrDataset")
}


#' Title
#'
#' @param a
#'
#' @return
#' @export
#'
#' @examples
#' dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
#' ds <- GDALOpen(dsn)
#' GDALGetInfo(ds)
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
#' @return
#' @export
#'
#' @examples
GDALGetInfo <- function(x) {
  .Call("GdalPtrGetInfo", x)
}
new_env <- function() {
  new.env(parent = emptyenv())
}
