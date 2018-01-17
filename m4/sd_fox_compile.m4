# Define a couple of macros to help end users
# determine if they need to install the FOX libs.
#
# Try to compile a very simple FOX app that calls
# the default constructor of the FXApp object, and
# immediately throws it away. If this object can't be
# called, a FOX app can't run.

AC_DEFUN([SIMO_FOX_COMPILE],
[# FOX Version: 1.6

ac_fox_compile="$CXX -g -c FoxTest.cpp"
cat > FoxTest.cpp << EOF
#include <fox-1.6/fx.h>
int main(void)
{
FXApp();
return 0;
}
EOF

# Compile test: must have FOX headers for this. Use system path
# to FOX header files.

if { (eval echo "$as_me:$LINENO: \"$FoxCompile\"") >&5
(eval "$ac_fox_compile") 2>&5
ac_status=$?
echo "$as_me:$LINENO: \$? = $ac_status" >&5
(eval echo "$as_me:$LINENO: Run FoxTest") >&5
(exit $ac_status); }
then
AC_MSG_RESULT([====> Compile test successful <====])
else
rm -f FoxTest.cpp
cat << EOF

!!!!!!!!!!!!!!!! Configure Error !!!!!!!!!!!!!!!!!!!!!!
!!                                                   !!
!!  FOX libs not found on this system                !!
!!  Please install FOX from the included "tarball"   !!
!!  Then re-run configure. (See "INSTALL" for more   !!
!!  details.)                                        !!
!!                                                   !!
!!!!!!!!!!!!!!!! Configure Error !!!!!!!!!!!!!!!!!!!!!!

EOF
exit 1
fi])