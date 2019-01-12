<?php if($personel_location_selected<>14) { ?>
<a href="income.php?action=add" accesskey="a">
  <div class="main_menu">
     <p><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd Income</p>
  </div>
</a>
<?php } ?>

<?php if($personel_location_selected==14) { ?>
<a href="empress_income.php?action=add&ontripsell=1" accesskey="a">
  <div class="main_menu">
     <p><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd "on trip" Sell</p>
  </div>
</a>
<?php } ?>



<a href="costs.php?action=add">
  <div class="main_menu">
     <p><i class="fa fa-plus" aria-hidden="true"></i> Add Costs</p>
  </div>
</a>
<a href="report_inc_daily.php?date=<?php echo date("Y-m-d"); ?>">
    <div class="main_menu">
     <p><i class="fas fa-chart-line" aria-hidden="true"></i> Income daily Reports</p>
  </div>
</a>



<div class="footer">

<?php

$date = new DateTime("now", new DateTimeZone('Asia/Makassar') );
echo '<h2>Time in Asia/Makassar: '.$date->format('Y-m-d H:i:s').'</h2>';

$date = new DateTime("now", new DateTimeZone('Europe/Warsaw') );
echo '<h2>Time in Europe/Warsaw: '.$date->format('Y-m-d H:i:s').'</h2>';

?>
</div>


