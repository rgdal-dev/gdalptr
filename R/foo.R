#' Title
#'
#' @return
#' @export
#'
#' @examples
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
GDALOpen<- function(dsn = NULL) {
  
  ds <- GDALDataset()
  if (is.null(dsn)) dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
  if (file.exists(dsn)) dsn <- normalizePath(dsn)
    .Call("GdalPtrOpen", ds, dsn)
}


#' Title
#'
#' @param x
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
