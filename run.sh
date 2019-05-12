if test -x python &>/dev/null; then
  echo "python found"
else
  echo "Loading python"
  spack load python
fi

rm -rf ~/master_project/traindata/*
source ~/master_project/dxt-vis/venv/bin/activate
pip install -r ~/master_project/dxt-vis/requirements_dev.txt
pip install Pillow

export EXPORT_PNG_PATH=~/master_project/traindata/
export PYTHONPATH=$PYTHONPATH:~/master_project/dxt-vis
export DXT_ENABLE_IO_TRACE=true

if test -x h5pcc &>/dev/null; then
  echo "hdf5 found"
else
  echo "Loading hdf5"
  spack load hdf5
fi
if test -x mpicc &>/dev/null; then
  echo "mpi found"
else
  echo "loading mpi"
  spack load mpi
fi

make

for i in {1..100}
do
  DIM0=$(( $RANDOM % 2000 + 100 ))
  DIM1=$(( $RANDOM % 2000 + 100 ))
  DSET=$(( 2 + $RANDOM % 8 ))
  PATTERN=1
  echo "DIMENSIONS: $DIM0 $DIM1"
  echo "DATASETS: $DSET"
  # create a new h5 file and write some data to it
  mpiexec -n 1 ./main $DSET $DIM0 $DIM1 1 0

  echo "Tracing ..."
  if [ $PLATFORM == Linux ]; then
    # execute with darshan
    mpiexec -n 1 -env LD_PRELOAD $(pwd)/../usr/local/lib/libdarshan.so \
      ./main $DSET $DIM0 $DIM1 2 $PATTERN
    DAY=$(date -d "$D" '+%-d')
    MONTH=$(date -d "$D" '+%-m')
    YEAR=$(date -d "$D" '+%Y')
    FOLDER=$(pwd)"/../d_logs/$YEAR/$MONTH/$DAY"
    LATEST_LOG=$(pwd)"/../d_logs/$YEAR/$MONTH/$DAY/"
    LATEST_LOG+=$(ls -Art $(pwd)"/../d_logs/$YEAR/$MONTH/$DAY" | tail -n 1)
    NOW=$(date "+%m-%d-%y_%H:%M:%S")
    NEWOUT="io_"$NOW"_trace.png"
    echo -e "\nParsing dxt file\n"
    python -m darshan_dxt_timeline.dxt2groups --export-png --export-png-path \
      $EXPORT_PNG_PATH$NOW $LATEST_LOG
    echo -e "Latest Log $LATEST_LOG\n"
    rm -rf $(pwd)/../d_logs/$YEAR/$MONTH/$DAY/*
  else
    echo "Platform not supported"
  fi
done
