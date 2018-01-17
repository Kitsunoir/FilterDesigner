# Macro to link and run a very simplistic FOX app.
# If this test fails, it means that either the FOX
# shared libs are corrupt, missing, or most likely
# ld can't find them because they're either in some
# wierd location, or weren't registered with ld.
#
# After installing FOX, run /sbin/ldconfig to register
# the new libs. (Some distros (Slackware) do this on
# reboot, others do not.) If this test succeeds, you're
# good to go!

AC_DEFUN([SIMO_FOX_RUN],
[ac_fox_link="$CXX -lFOX-1.6 -o FoxTest FoxTest.o"
rm -f FoxTest.cpp
if { (eval echo "$as_me:$LINENO: \"$FoxLink\"") >&5
(eval "$ac_fox_link") 2>&5
ac_status=$?
echo "$as_me:$LINENO: \$? = $ac_status" >&5
(./FoxTest) 2>&5
ac_status=$?
echo "$as_me:$LINENO: \$? = $ac_status" >&5
(exit $ac_status); }
then
AC_MSG_RESULT([====> Linked and ran test program successfully <====])
FoxLibs="-lFOX-1.6"
else
cat << EOF

!!!!!!!!!!!!!!!! Configure Error !!!!!!!!!!!!!!!!!!!!!!
!!                                                   !!
!!  Link test failed. FOX libs are present, however, !!
!!  they were likely not registered with ld after    !!
!!  doing the install. See the file INSTALL for      !!
!!  details on installing FOX and registering these  !!
!!  libs so that ld can find them without specifying !!
!!  a path to the FOX libs. (Essential in order to   !!
!!  easily launch the app!)                          !!
!!                                                   !!
!!!!!!!!!!!!!!!! Configure Error !!!!!!!!!!!!!!!!!!!!!!
EOF

rm -f FoxTest.o
exit 1
fi

rm -f FoxTest.o
rm -f FoxTest

AC_SUBST(FoxLibs)])