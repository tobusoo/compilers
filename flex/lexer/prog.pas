const Sz = 100;
var
  a: array [1..Sz] of integer;
  N: integer;
  i: integer;
begin
  readln(N);
  for i:=1 to N do
    read(a[i]);
  Writeln('A:');
  for i:=1 to N do
    write(a[i]);
end.
