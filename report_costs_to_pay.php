
<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');


?>

<?php

echo '<h1 align=center>Costs - still to pay</h1>';

echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
     <th>Date</th>
     <th>Name</th>
     <th>Location</th>
     <th>Personel</th>
     <th>Description</th>
     <th>Still to pay</th>
     <th>Amount Paid</th>
  </tr>
</thead>';


if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';



$stmt = $dbh->prepare("SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, cash_book.date,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name
                              FROM (personel, location, cash_book_group,cash_book)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              ".$sel_loc_pers."
                              ".$location_limit."
                              ".$period_limit."
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id

                              and cash_book.value<cash_book.costs_bill_value

                              ORDER BY cash_book.date desc");
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $route_txt = '';
   if($row[boat_trip_id]>0)
      $route_txt = '<br><font size=1vw color=grey>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'</font>';


   echo '<tr>
   <td align=center>'.$row['date'].'</td>
   <td>'.$row['cb_name'].'</td>
   <td>'.$row['loc_name'].'</td>
   <td>'.$row['pers_name'].'</td>
   <td>'.$row['desc'].$route_txt.'</td>';

   if($row['costs_bill_value']<>0 and ($row['value']-$row['costs_bill_value'])<>0)
      {
       $still_to_pay = $formatter->formatCurrency($row['costs_bill_value']-$row['value'], 'IDR');
       $sum_still += $row['costs_bill_value']-$row['value'];
       }
   else
       $still_to_pay = '';

   echo '<td align=right>'.$still_to_pay.'</td>';

   echo '
   <td align=right>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>
   </tr>';

   $sum_value += $row['value'];
   }


?>

<tr>
 <td colspan=5></td>
 <td class=cash><?php echo $formatter->formatCurrency($sum_still, 'IDR'); ?></td>
 <td class=cash><?php echo $formatter->formatCurrency($sum_value, 'IDR'); ?></td>
</tr>

</table>


<?php include('inc/footer.inc'); ?>