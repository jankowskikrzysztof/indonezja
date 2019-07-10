      <style>
         tr {
             font-size: 0.8em;
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


//if($_SESSION['limit_data_months']<>0)
//  {
//  echo 'Period limit';
//  exit;
//  }

?>

<script>
function changePeriod(url)
{
    location.href = 'report_cb_group_year.php?senkut=<?php echo $_GET['senkut']; ?>&period=' + url;
}
</script>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

<?php

echo '<h2 align=center>Annual Costs Report: '.$period_year.'</h2>';


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
	


if($personel_location_selected <> '' and $action<>'edit')
	$def_location = 'and location.id_location IN ('.$personel_location_selected.')';
else
	$def_location = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

	  //	  $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
if($_SESSION['limit_data_months']<>0)
	  $period_limit = 'and cash_book.date >=  DATE_SUB(DATE_FORMAT(CURRENT_DATE,"%Y-%m-01"), INTERVAL '.$_SESSION['limit_data_months'].' month)';
else
      $period_limit = '';

//<tr><td colspan=14 align=right><a href="?period='.$period.'&senkut=1">Show only Senggigi and Kuta</a></td></tr>
echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
   
<tr><td align=center>Group - Name</td>';

	for($m=1;$m<=12;$m++)
	   {
	   $month_name = DateTime::createFromFormat('!m', $m);
	   echo '<td align=center>'.$month_name->format('F').'</td>';
	   }

echo '<td align=center>TOTAL</td></tr></thead>';

/*
$stmt = $dbh->prepare("SELECT location.id_location, location.name, location_groups.name as group_name, location.room_charge_perc 
FROM `location`, location_groups 
WHERE location.location_groups_id=location_groups.id_location_groups 
                              ".$def_location."
                              ".$location_limit."
ORDER BY group_name, name");
*/

$stmt = $dbh->prepare("SELECT cash_book_group.* FROM cash_book_group
WHERE type=2
and id_cash_book_group NOT IN (21,22,23,24)
ORDER BY name");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['name'].'</td>';


$select = "SELECT YEAR(cash_book.date) as year, MONTH(cash_book.date) as month, cash_book.location_id, location.name, 

SUM(case when cash_book.type=1 then cash_book.value else 0 end) as sum_income,
SUM(case when cash_book.type=2 then cash_book.value else 0 end) as sum_costs,

cash_book.*

FROM `cash_book`, `location`

WHERE cash_book.location_id=location.id_location
and cash_book_group_id NOT IN (21,22,23,24) 
and YEAR(cash_book.date)=".$period_year."

and cash_book.cash_book_group_id=".$row['id_cash_book_group']."
			".$def_location."
			".$location_limit."
			".$kuta_seng_location."
			".$location_limit_sel."
			".$period_limit."
GROUP BY YEAR(cash_book.date), MONTH(cash_book.date), cash_book.cash_book_group_id

ORDER BY YEAR(cash_book.date), MONTH(cash_book.date), location.name";

//echo $select;

	$stmt_income = $dbh->prepare("

$select

			");
	$stmt_income -> execute();
	$row_array_income = $stmt_income->fetchAll();


//	echo '<td><pre>';
//var_dump($row_array_costs);
//	echo '</pre></td>';

$sum_loc_costs = 0;
$array_pos = 0;

//	for($array_pos=0;$array_pos<12;$array_pos++)

	for($m=1;$m<=12;$m++)
	   {

//echo $array_pos.'bb'.$m.'aa'.$row_array_income[$array_pos][month].'<br>';

	   if($m == $row_array_income[$array_pos][month])
		{
		   echo '<td class=cash>
						'.$formatter->formatCurrency($row_array_income[$array_pos][sum_costs], 'IDR').'
					</td>';
		   
		   $sum_costs[$m] += $row_array_income[$array_pos][sum_costs];

		   $sum_loc_costs += $row_array_income[$array_pos][sum_costs];

		$array_pos++;
		}
	   else
		echo '<td></td>';
		//echo '<td>'.$m.' - '.$array_pos.'</td>';

	   //$array_pos++;
	   }

   $sum_loc_res = $sum_loc_income - $sum_loc_costs;

   echo '<td class=cash>'.$formatter->formatCurrency($sum_loc_costs, 'IDR').'</td>';


   echo '</tr>';


   }


   echo '<tr><td>TOTAL</td>';

	for($m=1;$m<=12;$m++)
	   {
	   $profit = $sum_income[$m]-$sum_costs[$m];

	   echo '<td class=cash>'.$formatter->formatCurrency(round($sum_costs[$m]), 'IDR').'</td>';
	   
	   $sum_loc_income += $sum_income[$m];
	   $sum_loc_costs += $sum_costs[$m];

	   }

	$profit = $sum_loc_income-$sum_loc_costs;

  echo '<td class=cash>'.$formatter->formatCurrency(round($sum_loc_costs), 'IDR').'</td>';

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




?>




<?php include('inc/footer.inc'); ?>
