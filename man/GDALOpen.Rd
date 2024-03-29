% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/gdalptr.R
\name{GDALOpen}
\alias{GDALOpen}
\title{Open a GDAL dataset from a dsn}
\usage{
GDALOpen(dsn = NULL, read_only = TRUE)
}
\arguments{
\item{dsn}{character, description of a file path, url, etc.}

\item{read_only}{default is \code{TRUE} open the dsn in read-only mode}
}
\value{
an external pointer
}
\description{
Open a GDAL dataset from a dsn
}
\examples{
dsn <- system.file("extdata/volcano_gcp.tif", package = "gdalptr", mustWork  = TRUE)
ds <- GDALOpen(dsn)
GDALGetRasterSize(ds)
}
