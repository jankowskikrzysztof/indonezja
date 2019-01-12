<?php
$debug = 1;

if($debug == 0 and $_SESSION['login']=='kjankowski')
          {

//if(isset($_SESSION['screen_width']) AND isset($_SESSION['screen_height'])){
//    echo 'User resolution: ' . $_SESSION['screen_width'] . 'x' . $_SESSION['screen_height'];
//}

//echo $_SERVER['HTTP_USER_AGENT'] . "\n\n";

          echo '<br><br><hr>'.$_SERVER['REMOTE_ADDR'].'<pre>';
          print_r(error_get_last());
          echo '<br><br><hr>';
          print '<pre>' . htmlspecialchars(print_r(get_defined_vars(), true)) . '</pre>';
          }
?>
<div class="footer">
<!--  <p>(c) GruVi</p>-->
</div>
</body>
</html>