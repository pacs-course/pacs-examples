

fmt = ...
'make CXX="g++" CPPFLAGS="-I. -Umsize -Dmsize=%d %s" distclean test_matrix_mult\n';

m   = [];
mm  = [];
mmm = [];
d   = [];

for ii = 1 : 11

  printf (fmt, 2^ii, "");
  [a, str] = system (sprintf (fmt, 2^ii, ""));
  [a, str] = system ('./test_matrix_mult');
  if (a != 0)
    disp (str)
    return
  endif
  eval (str);
  d = [d; msize];
  m = [m; multiply_time];

  printf (fmt, 2^ii, "-DMAKE_TMP_TRANSP");
  [a, str] = system (sprintf (fmt, 2^ii, "-DMAKE_TMP_TRANSP"));
  [a, str] = system ('./test_matrix_mult');
  if (a != 0)
    disp (str)
    return
  endif
  eval (str);
  mm = [mm; multiply_time];

  printf (fmt, 2^ii, "-DUSE_DGEMM");
  [a, str] = system (sprintf (fmt, 2^ii, "-DUSE_DGEMM"));
  [a, str] = system ('./test_matrix_mult');
  if (a != 0)
    disp (str)
    return
  endif
  eval (str);
  mmm = [mmm; multiply_time];

  loglog (d, m, 'linewidth', 5,
          d, mm, 'linewidth', 5,
          d, mmm, 'linewidth', 5);
  
  set (gca (), "FontSize", 18)
  legend ("without transpose", "with transpose", "whith dgemm",
          "location", "southoutside")
  xlabel ("matrix size", "FontSize", 18)
  ylabel ("multiply time [ms]", "FontSize", 18)
  axis tight
  drawnow
endfor

print -dpdf speedup.pdf

