<?php

$id_boat_trip = $_GET['id_boat_trip'];

echo '<h1 align=center>Trip result</h1>';

$stmt = $dbh->prepare("SELECT boat_trip.id_boat_trip,
                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route,
                              YEAR(boat_trip.date_to) as trip_year,
                              MONTH(boat_trip.date_to) as trip_month,
                              boat_trip.clearance_scan_org_filename_ext


                              FROM boat_trip
                              WHERE
                                  boat_trip.id_boat_trip=".$id_boat_trip."

                              ORDER BY boat_trip.date_to desc");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $date1 = new DateTime($row['date_from']);
   $date_trip_from = $row['date_from'];
   $date2 = new DateTime($row['date_to']);
   $date_trip_to = $row['date_to'];
   $clearance_scan_org_filename_ext = $row['clearance_scan_org_filename_ext'];

   $costs_period_month = $row['trip_month'];
   $costs_period_year = $row['trip_year'];
   $costs_last_day = date("t", strtotime($row['date_to']));
   $costs_month_day = date("F", strtotime($row['date_to']));

   $numberOfNights = $date2->diff($date1)->format("%a");
   $numberOfDays = $numberOfNights+1;
   $trip = $row['txt_route'].' from '.$row['date_from'].', to: '.$row['date_to'].', days: '.$numberOfDays;
   }

// jeden trip wczesnie - potrzebny zeby wyciagnac date zakonczenia ostatniego tripu do wyliczenia dziennej pensji w przestoju ktora dodawana jest do kosztow biezacego tripu

$stmt = $dbh->prepare("SELECT boat_trip.id_boat_trip,
                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route,
                              YEAR(boat_trip.date_to) as trip_year,
                              MONTH(boat_trip.date_to) as trip_month


                              FROM boat_trip
                              WHERE
                                  boat_trip.date_to<='".$date_trip_from."'
                              and boat_trip.id_boat_trip<>".$id_boat_trip."

                              ORDER BY boat_trip.date_to DESC
                              LIMIT 1");

$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $date_to_last_trip = $row['date_to'];
   $date1 = new DateTime($row['date_to']);

   $numberOfNightsLastEndThis = $date2->diff($date1)->format("%a");
   $numberOfDaysFromLastTripToThisEnd = $numberOfNightsLastEndThis+1;
   }


echo '<h2 align=center>'.$trip.'</h2>';

echo '<table width=80%><tr valign=top><td width=50%>';   //tabelka do ulozenia wszystkiego na stronie

echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<tr><td colspan=2 align=center><h3>Income</h3>
<p align=right>Clearance List: &nbsp;'; 
      if(file_exists('img_uploads/boat_trip/clearance_list_'.$id_boat_trip.'.'.$clearance_scan_org_filename_ext))
         echo '<a href="img_uploads/boat_trip/clearance_list_'.$id_boat_trip.'.'.$clearance_scan_org_filename_ext.'" target=_blank><i class="far fa-file-alt fa-2x"></i></a>';

echo '</p></td></tr>
<tr><td align=center>Name</td>
<td>Income</td>
</tr>
</thead>';


$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,
                              activity.shortcut,
                              activity.name as act_name,
                              client.name as client_name,
                              `cash_book_item`.value as item_value_before_discount,
                              `cash_book`.booker_perc,
                              ROUND(`cash_book_item`.value-`cash_book_item`.value*`cash_book`.booker_perc/100) as item_value,


                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM (personel, location, cash_book_group,cash_book, cash_book_item,activity)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              left join client on client.id_client=cash_book.client_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.id_cash_book=cash_book_item.cash_book_id
                              and activity.id_activity=cash_book_item.activity_id
                              and cash_book.personel_id=personel.id_personel
                              and boat_trip.id_boat_trip=".$id_boat_trip."
                              and cash_book.type=1
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              ORDER BY client.name, activity.shortcut
");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['client_name'].' - '.$row['shortcut'].' - '.$row['act_name'].'</td><td class=cash>'.$formatter->formatCurrency($row['item_value'], 'IDR').'</td>';


   $sum_income += $row['item_value'];

   echo '</tr>';


   }

echo '<tfoot>
<tr><td align=right>Sum:</td>
<td class=cash><b>'.$formatter->formatCurrency($sum_income, 'IDR').'</b></td>
</tr>
</tfoot>';

echo '</table>';


echo '<br><br>';


echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<tr><td colspan=2 align=center><h3>Costs</h3></td></tr>
<tr><td align=center>Name</td>
<td>Paid</td>
</tr>
</thead>';


$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM (personel, location, cash_book_group,cash_book)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and boat_trip.id_boat_trip=".$id_boat_trip."
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              and cash_book_group.name<>'Salaries'
                              ORDER BY cash_book_group.name asc, cash_book.desc
");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td>'.$row['cb_name'].' - '.$row['desc'].'</td><td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';


   $sum_costs += $row['value'];

   echo '</tr>';


   }



echo '<tfoot>
<tr><td align=right>Sum:</td>
<td class=cash><b>'.$formatter->formatCurrency($sum_costs, 'IDR').'</b></td>
</tr>
</tfoot>';

echo '</table>';



//echo '<br><br>';
echo '</td><td width=50%>';   //tabelka do ulozenia wszystkiego na stronie


echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<tr><td colspan=2 align=center><h3>Salaries</h3></td></tr>
<tr><td align=center>Crew</td>
<td>Paid</td>
</tr>
</thead>';


$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM (personel, location, cash_book_group,cash_book)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel

                              and cash_book.location_id=14
                              and cash_book.cash_book_group_id=4

                              and YEAR(cash_book.date)=".$costs_period_year."
                              and MONTH(cash_book.date)=".$costs_period_month."
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id

                              and ( cash_book.boat_trip_id=0 or cash_book.boat_trip_id= ".$id_boat_trip.")

                              ORDER BY cash_book.boat_trip_id desc, cash_book.date desc
");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $sal_type = '';
   if($row['boat_trip_id'] == 0)
      $sal_type = '[monthly]';
   elseif($row['boat_trip_id'] <> 0)
      $sal_type = '[trip]';

   echo '<tr>
   <td>'.$row['cb_name'].' - '.$row['desc'].' '.$sal_type.'</td><td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';


   if($row['boat_trip_id'] == 0)
     $sum_salaries_month += $row['value'];
   if($row['boat_trip_id'] <> 0)
     $sum_salaries_trip += $row['value'];

   echo '</tr>';


   }

$total_salary_per_day = round($sum_salaries_month/$costs_last_day);
$total_salary_per_trip = round($total_salary_per_day*$numberOfDaysFromLastTripToThisEnd);

echo '<tfoot>
<tr><td align=right>Total trip salary crew:</td>
<td class=cash><b>'.$formatter->formatCurrency($sum_salaries_trip, 'IDR').'</b></td>
</tr>

<tr><td align=right>Total monthly salary crew ('.$costs_month_day.'):</td>
<td class=cash><b>'.$formatter->formatCurrency($sum_salaries_month, 'IDR').'</b></td>
</tr>

<tr><td align=right>Salary per day ('.$costs_last_day.' days):</td>
<td class=cash><b>'.$formatter->formatCurrency($total_salary_per_day, 'IDR').'</b></td>
</tr>

<tr><td align=right>Avg. Salary per trip ('.$numberOfDaysFromLastTripToThisEnd.' days):</td>
<td class=cash><b>'.$formatter->formatCurrency($total_salary_per_trip, 'IDR').'</b></td>
</tr>


</tfoot>';

echo '</table>';

$trip_result = $sum_income - $sum_costs - $total_salary_per_trip - $sum_salaries_trip;

echo '<br><br><h2 align=center>Trip summary:</h2>';
echo '<table class="pure-table pure-table-horizontal pure-table-striped">
         <tr><td align="right">'.$formatter->formatCurrency($sum_income, 'IDR').'</td><td>Trip income</td></tr>
         <tr><td align="right">'.$formatter->formatCurrency($sum_costs, 'IDR').'</td><td>Trip costs</td></tr>
         <tr><td align="right">'.$formatter->formatCurrency($sum_salaries_trip, 'IDR').'</td><td>Salary trip selected</td></tr>
         <tr><td align="right">'.$formatter->formatCurrency($total_salary_per_trip, 'IDR').'</td><td>Salary avg. per trip</td></tr>
         <tr><td align="right"><b>'.$formatter->formatCurrency($trip_result, 'IDR').'</b></td><td><b>Trip result</b></td></tr>
      </table>';
      
// echo '<p align=center>'.$formatter->formatCurrency($sum_income, 'IDR').' - Trip income</p>';
// echo '<p align=center>'.$formatter->formatCurrency($sum_costs, 'IDR').' - Trip costs</p>';
// echo '<p align=center>'.$formatter->formatCurrency($sum_salaries_trip, 'IDR').' - Salary trip selected</p>';
// echo '<p align=center>'.$formatter->formatCurrency($total_salary_per_trip, 'IDR').' - Salary avg. per trip</p>';
// echo '<h2 align=center>'.$formatter->formatCurrency($trip_result, 'IDR').' - Trip result</h3>';


echo '</td></tr></table>';   //tabelka do ulozenia wszystkiego na stronie


?>