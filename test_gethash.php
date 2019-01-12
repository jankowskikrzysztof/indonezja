<?php

/*
GetHash(string s)
{
long h = 7;
for (int i = 0; i < s.Length; i++)
h = h * 37 + "acegilmnoprstuwxyz".IndexOf(s[i]);
return h;
}
*/

$h=7;

for ($i=0; $i< strlen($s); $i++)
  {
    $h = $h*37 + strpos("acegilmnoprstuwxyz",strlen($s));
  }


?>