<?php


/**
 * @filename: currentgitbranch.php
 * @usage: Include this file after the '<body>' tag in your project
 * @author Kevin Ridgway 
 */
$stringfromfile = file('.git/HEAD', FILE_USE_INCLUDE_PATH);

$firstLine = $stringfromfile[0]; //get the string from the array

$explodedstring = explode("/", $firstLine, 3); //seperate out by the "/" in the string

$branchname = $explodedstring[2]; //get the one that is always the branch name

$debug = 1;

if($debug == 1 and $_SESSION['login']=='kjankowski')
          {

//if(isset($_SESSION['screen_width']) AND isset($_SESSION['screen_height'])){
//    echo 'User resolution: ' . $_SESSION['screen_width'] . 'x' . $_SESSION['screen_height'];
//}

//echo $_SERVER['HTTP_USER_AGENT'] . "\n\n";

/*
          echo '<br><br><hr>'.$_SERVER['REMOTE_ADDR'].'<pre>';
          print_r(error_get_last());
          echo '<br><br><hr>';
          print '<pre>' . htmlspecialchars(print_r(get_defined_vars(), true)) . '</pre>';
*/

          echo "<br><br><br><div style='clear: both; width: 100%; font-size: 14px; font-family: Helvetica; color: #30121d; background: #bcbf77; padding: 20px; text-align: center;'>Current branch: <span style='color:#fff; font-weight: bold; text-transform: uppercase;'>" . $branchname . "</span></div>"; //show it on the page

        }
?>
<div class="footer">
<!--  <p>(c) GruVi</p>-->
</div>
</body>
</html>