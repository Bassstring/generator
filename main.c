#include "pattern.h"
#include <hdf5.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define FILE "synth.h5"

int DIM0, DIM1, DSIZE;

void execute_pattern(hid_t *dset_arr, int pattern, int dsize, int d0, int d1) {

  char *label;
  printf("\nPattern %d will be executed\n", pattern);
  switch (pattern) {
  case 1:
    label = pattern1(dset_arr, dsize, d0, d1);
    break;
  case 2:
    label = pattern2(dset_arr, dsize, d0, d1);
    break;
  case 3:
    label = pattern3(dset_arr, dsize, d0, d1);
    break;
  default:
    break;
  }
  return;
}

int main(int argc, char *argv[]) {

  int rank, option, pattern;
  char hostname[256];
  char *basename = "dset";

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  gethostname(hostname, 255);
  DSIZE = atoi(argv[1]);
  DIM0 = atoi(argv[2]);
  DIM1 = atoi(argv[3]);
  option = atoi(argv[4]);
  pattern = atoi(argv[5]);

  hid_t file, space, dset;
  herr_t status;
  hsize_t dims[2] = {DIM0, DIM1};
  hid_t dset_arr[DSIZE];

  switch (option) {
  case 1:
    printf("Hello world!  I am process number: %d on host %s\n", rank,
           hostname);
    printf("Create file with datasets and data\n");

    file = H5Fcreate(FILE, H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    space = H5Screate_simple(2, dims, NULL);

    // create new datasets in the root group
    for (int i = 0; i < DSIZE; ++i) {
      char *new_name;
      asprintf(&new_name, "%s%d", basename, i);
      dset = H5Dcreate(file, new_name, H5T_STD_I32LE, space, H5P_DEFAULT,
                       H5P_DEFAULT, H5P_DEFAULT);

      int data[DIM0][DIM1];
      for (int k = 0; k < DIM0; k++)
        for (int l = 0; l < DIM1; l++)
          data[k][l] = k * l;

      status =
          H5Dwrite(dset, H5T_NATIVE_INT, H5S_ALL, H5S_ALL, H5P_DEFAULT, data);
      dset_arr[i] = dset;
    }

    status = H5Dclose(dset);
    status = H5Sclose(space);
    status = H5Fclose(file);
    break;
  case 2:
    printf("PATTERN\n");
    file = H5Fopen(FILE, H5F_ACC_RDWR, H5P_DEFAULT);
    for (int i = 0; i < DSIZE; ++i) {
      char *new_name;
      asprintf(&new_name, "%s%d", basename, i);
      dset = H5Dopen2(file, new_name, H5P_DEFAULT);
      dset_arr[i] = dset;
    }
    execute_pattern(dset_arr, pattern, DSIZE, DIM0, DIM1);
    status = H5Dclose(dset);
    status = H5Fclose(file);
    break;
  }
  MPI_Finalize();
  return 0;
}
