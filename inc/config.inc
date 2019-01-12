<?php

// Test the session to see if is_auth flag was set (meaning they logged in successfully)
// If test fails, send the user to login.php and prevent rest of page being shown.
if (!isset($_SESSION["is_auth"]) and $public<>1) {
	header("location: login.php");
	exit;
}
else if (isset($_REQUEST['logout']) && $_REQUEST['logout'] == "true") {
	// At any time we can logout by sending a "logout" value which will unset the "is_auth" flag.
	// We can also destroy the session if so desired.
	unset($_SESSION['is_auth']);
	session_destroy();

	// After logout, send them back to login.php
	header("location: login.php");
	exit;
}






include('inc/db_config.inc');

//include('inc/func_currency.inc');

$formatter = new NumberFormatter('id_ID',  NumberFormatter::DECIMAL);


if($action<>'xlse')
  {
?>

<script>
function loadPage(url)
{
    location.href = url;
}
</script>





<!-- skrypt umozliwiajacy uzywanie input date w firefox oraz ie) -->
<!--<script type="text/javascript" src="https://code.jquery.com/jquery-2.1.4.min.js"></script> -->
<script type="text/javascript" src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
<script type="text/javascript" src="https://cdnjs.cloudflare.com/ajax/libs/jquery.mask/1.14.15/jquery.mask.min.js"></script>

<!--<script defer src="https://use.fontawesome.com/releases/v5.0.9/js/all.js" integrity="sha384-8iPTk2s/jMVj81dnzb/iFR2sdA7u06vHJyyLlAd4snFpCl/SnyUjRrbdJsw1pGIl" crossorigin="anonymous"></script>-->

<script defer src="inc/fontawesome-free-5.1.0-web/js/all.js"></script>

<script src="//cdn.jsdelivr.net/webshim/1.16.0/polyfiller.js"></script>
<script>
webshims.setOptions('forms-ext', {types: 'date'});
webshims.polyfill('forms forms-ext');
$.webshims.formcfg = {
en: {
    dFormat: '-',
    dateSigns: '-',
    patterns: {
        d: "yy-mm-dd"
    }
}
};


$(document).ready(function(){
  $('.money').mask('000,000,000,000,000', {reverse: true});
});


$(document).ready(function(){
  $('.money5').mask('###0', {reverse: true});
});


$(document).keyup(function(){
  $('.money2').mask('000,000,000,000,000', {reverse: true});
});

$(document).val(function(){
  $('.money3').mask('000,000,000,000,000', {reverse: true});
});


</script>

<?php
  };
?>