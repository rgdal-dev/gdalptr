#' Title
#'
#' @param a
#'
#' @return
#' @export
#'
#' @examples
Rfun <- function(a = 1) {
    .Call(GdalPtrLoadDataset, "data-raw/volcano_gcp.tif", "cookies.txt")
}


new_env <- function() {
  new.env(parent = emptyenv())
}
