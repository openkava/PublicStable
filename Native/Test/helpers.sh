tmpfilename()
{
  if [ -n "$(which md5)" ]; then
    echo "/tmp/$(dd if=/dev/urandom bs=1024 count=1 2>/dev/null | md5).out"
  elif [ -n "$(which md5sum)" ]; then
    echo "/tmp/$(dd if=/dev/urandom bs=1024 count=1 2>/dev/null | md5sum | cut -c1-32).out"
  fi
}
