#  Define a macro that tries to run unisolve --
#  an outside app that filterdesigner uses to
#  solve for roots of Besel polynomials. Though
#  not strictky required, it does add functionality

AC_DEFUN([SIMO_MPSOLVE_FIND],
[ac_unisolve_run="unisolve polyfmt.mps"
cat > polyfmt.mps << EOF
sri
0
4
3
0
5
1
2
4
1
EOF

if { (eval echo "$as_me:$LINENO: \"Try to run unisolve\"") >&5
(eval "$ac_unisolve_run" > /dev/null) 2>&5
ac_status=$?
echo "$as_me:$LINENO: \$? = $ac_status" >&5
(eval echo "$as_me:$LINENO: Ran unisolve") >&5
(exit $ac_status); }
then
AC_MSG_RESULT([=====> Ran MPSolve successfully <=====])
AC_DEFINE([HAVE_MPSOLVE],[1])
else
AC_MSG_RESULT([MPSolve not found. This will impair functionality])
AC_DEFINE([HAVE_MPSOLVE],[0])
fi
rm -f polyfmt.mps])
