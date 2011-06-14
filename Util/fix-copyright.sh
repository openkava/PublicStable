#!/bin/sh
#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#

for filename in "$@"
do
  if ! head -4 "$filename" | grep -q 'Copyright 2010-2011 Fabric Technologies Inc\. All rights reserved\.'
  then
    echo "Fixing $filename"
    ext=${filename##*.}
    if [ "$extension" == "js" ]; then
      echo '
//
// Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
//
' >"$filename.$$"
    elif [ "$extension" == "cpp" -o "$extension" == "h" ]; then
      echo '
/*
 *  Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
 */
' >"$filename.$$"
    elif [ "$extension" == "sh" ]; then
      echo '
#!/bin/sh
#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#
' >"$filename.$$"
    else
      echo '
#
# Copyright 2010-2011 Fabric Technologies Inc. All rights reserved.
#
' >"$filename.$$"
    fi
    cat "$filename" >>"$filename.$$"
    mv "$filename.$$" "$filename"
  fi
done
