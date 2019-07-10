      <style>
         tr {
             font-size: 80%;
         }
         tr.loc {
             font-size: 120%;
         }
      </style>

<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');

$period_month = substr($period,5,2);
$period_year = substr($period,0,4);

/*if($_SESSION['limit_data_months']<>0)
  {
  echo 'Period limit';
  exit;
  }
*/
?>



<script>
function changePeriod(url)
{
    location.href = 'report_year.php?senkut=<?php echo $_GET['senkut']; ?>&period=' + url;
}
</script>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

<?php

echo '<h2 align=center>Annual Total Report: '.$period_year.'</h2>';


/*
if($_GET['senkut'] == 1)
	{
	$kuta_seng_location = 'and location.id_location IN (6,7,8,10,12)';
	echo '<h3 align=center>Senggigi and Kuta Locations</h3>';
	}
else
	{
	$kuta_seng_location = '';
	}
*/

$loc_lists = implode(',', $_POST['loc_list']);
if($loc_lists<>'')
      $location_limit_sel = 'and location.id_location IN ('.$loc_lists.')';
else
	$location_limit_sel = '';

//      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';


if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  DATE_SUB(DATE_FORMAT(CURRENT_DATE,"%Y-%m-01"), INTERVAL '.$_SESSION['limit_data_months'].' month)';
else
      $period_limit = '';

if($personel_location_selected <> '' and $action<>'edit')
$def_location = 'and location.id_location IN ('.$personel_location_selected.')';
else
$def_location = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';


//   <tr><td colspan=15 align=right><a href="?period='.$period.'&senkut=1">Show only Senggigi and Kuta</a></td></tr>


echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
   <tr><td align=center>Group - Name</td><td></td>';

	for($m=1;$m<=12;$m++)
	   {
	   $month_name = DateTime::createFromFormat('!m', $m);
	   echo '<td align=center>'.$month_name->format('F').'</td>';
	   }

echo '<td align=center>TOTAL</td></tr></thead>';

$select = "SELECT location.id_location, location.name, location_groups.name as group_name, location.room_charge_perc 
FROM `location`, location_groups 
WHERE location.location_groups_id=location_groups.id_location_groups 
                              ".$def_location."
							  ".$location_limit."
							  ".$location_limit_sel."
							  ".$kuta_seng_location."
ORDER BY group_name, name";



$stmt = $dbh->prepare("$select");
$stmt -> execute();

$row_array = $stmt->fetchAll();


foreach($row_array as $row)
   {
	$rc_txt = '';
	$rc_info = '';
	if($row['room_charge_perc']<>0)
		{
		$rc_txt = '<br>RC: '.$row['room_charge_perc'].' %';
		$rc_info = '<br>roomcharge';
		}


   echo '<tr>
   <td>'.$row['name'].$rc_txt.'</td><td align=right>income'.$rc_info.'<br>costs<br>profit</td>';

$select_income = "				SELECT YEAR(cash_book.date) as year, MONTH(cash_book.date) as month, cash_book.location_id, location.name, location.room_charge_perc,

SUM(case when cash_book.type=1 then ROUND(cash_book.value) else 0 end) as sum_income,
SUM(case when cash_book.type=2 then ROUND(cash_book.value) else 0 end) as sum_costs,

SUM(case when cash_book.type=1 then ROUND(cash_book.value) else 0 end)*location.room_charge_perc/100 as sum_rc, 

cash_book.*

FROM `cash_book`, `location`

WHERE cash_book.location_id=location.id_location
and cash_book_group_id NOT IN (21,22,23,24) 
".$period_limit."

and YEAR(cash_book.date)=".$period_year."

and cash_book.location_id=".$row['id_location']."

GROUP BY YEAR(cash_book.date), MONTH(cash_book.date), cash_book.location_id

ORDER BY YEAR(cash_book.date), MONTH(cash_book.date), location.name";

	$stmt_income = $dbh->prepare("$select_income");

		//	echo $select_income.'<br>';

	$stmt_income -> execute();
	$row_array_income = $stmt_income->fetchAll();


//	echo '<td><pre>';
//var_dump($row_array_costs);
//	echo '</pre></td>';

$sum_loc_income = 0;
$sum_loc_rc = 0;
$sum_loc_costs = 0;
$array_pos = 0;


$sum_loc_rc_txt = '';

	for($m=1;$m<=12;$m++)
	   {

	   if($m == $row_array_income[$array_pos]['month'])
		{

		$sum_rc_loc_txt = '';

		if($row['room_charge_perc']<>0)
			{
			$sum_rc_loc_txt = $formatter->formatCurrency($row_array_income[$array_pos]['sum_rc'], 'IDR').'<br>';
			$sum_loc_rc_txt = '<br>';
			}

		$profit = $row_array_income[$array_pos]['sum_income']-$row_array_income[$array_pos]['sum_rc']-$row_array_income[$array_pos]['sum_costs'];

		echo '<td class=cash>'.$formatter->formatCurrency($row_array_income[$array_pos]['sum_income'], 'IDR').'<br>
						'.$sum_rc_loc_txt.'
						'.$formatter->formatCurrency($row_array_income[$array_pos]['sum_costs'], 'IDR').'<br>
						'.$formatter->formatCurrency($profit, 'IDR').'</td>';
		   
		   $sum_income[$m] += $row_array_income[$array_pos]['sum_income'];
		   $sum_rc[$m] += $row_array_income[$array_pos]['sum_rc'];
		   $sum_costs[$m] += $row_array_income[$array_pos]['sum_costs'];

		   $sum_loc_income += $row_array_income[$array_pos]['sum_income'];
		   $sum_loc_rc += $row_array_income[$array_pos]['sum_rc'];
		   $sum_loc_costs += $row_array_income[$array_pos]['sum_costs'];

		$array_pos++;
		}
	   else
		echo '<td></td>';

		}

   $sum_loc_res = $sum_loc_income - $sum_loc_rc - $sum_loc_costs;

   if($sum_loc_rc<>0) $sum_loc_rc_txt = $formatter->formatCurrency($sum_loc_rc, 'IDR').'<br>';


   echo '<td class=cash>'.$formatter->formatCurrency($sum_loc_income, 'IDR').'<br>
	   				'.$sum_loc_rc_txt.'
					'.$formatter->formatCurrency($sum_loc_costs, 'IDR').'<br>
					'.$formatter->formatCurrency($sum_loc_res, 'IDR').'</td>
					';


   echo '</tr>';


   }


   echo '<tr><td>TOTAL</td><td align=right>income<br>roomcharge<br>costs<br>profit</td>';

	for($m=1;$m<=12;$m++)
	   {
	   $profit = $sum_income[$m]-$sum_rc[$m]-$sum_costs[$m];

	   echo '<td class=cash>'.$formatter->formatCurrency(round($sum_income[$m]), 'IDR').'<br>
				   '.$formatter->formatCurrency(round($sum_rc[$m]), 'IDR').'<br>
	   				'.$formatter->formatCurrency(round($sum_costs[$m]), 'IDR').'<br>
					'.$formatter->formatCurrency(round($profit), 'IDR').'</td>';
	   
	   $sum_loc_income1 += $sum_income[$m];
	   $sum_loc_rc1 += $sum_rc[$m];
	   $sum_loc_costs1 += $sum_costs[$m];

	   }

	$profit1 = $sum_loc_income1-$sum_loc_rc1-$sum_loc_costs1;

  echo '<td class=cash>'.$formatter->formatCurrency(round($sum_loc_income1), 'IDR').'<br>
					'.$formatter->formatCurrency(round($sum_loc_rc1), 'IDR').'<br>
					'.$formatter->formatCurrency(round($sum_loc_costs1), 'IDR').'<br>
					'.$formatter->formatCurrency(round($profit1), 'IDR').'</td>';

   echo '</tr>';



echo '</table>';




foreach ($_POST['loc_list'] as $loc) {
			$loc_sel[$loc] = 1;
		 }


echo '<br><br><div align=center>
<table class="pure-table pure-table-horizontal pure-table-striped"><tr class=loc><td>
<form action="?period='.$period.'" method="post">
<input type=hidden name=period value='.$period.'>';


$stmt = $dbh->prepare("SELECT location.id_location, location.name, location_groups.name as group_name, location.room_charge_perc 
FROM `location`, location_groups 
WHERE location.location_groups_id=location_groups.id_location_groups 
                              ".$def_location."
							  ".$location_limit."
ORDER BY group_name, name");
$stmt -> execute();

$row_array = $stmt->fetchAll();
foreach($row_array as $row)
   {
	   if($loc_sel[$row['id_location']] == 1)
		    $input_checked = 'checked';
		else
			$input_checked = '';
		
	echo '<label><input type="checkbox" name="loc_list[]" value="'.$row['id_location'].'" '.$input_checked.'> '.$row['name'].'</label><br><br>';
   }

?>
</td></tr>
<tr class=loc><td align=center><input type="submit" value="show"></td></tr>
</form>
</table>
</div>




<?php include('inc/footer.inc'); ?>
