<script>
function changePeriod(url)
{
    location.href = 'costs.php?method=<?php echo $_GET['method'];?>&period=' + url;
}
</script>

<script>
function changePeriodMethod(url)
{
    location.href = 'costs.php?period=<?php echo $_GET['period'];?>&' + url;
}
</script>

<div align=center class="pure-form">

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>
<?php
if($_SESSION['permission']=='rw')
echo '  
      <th colspan=8><a class="pure-button pure-button-primary" href="?period='.$period.'&action=add" accesskey="a"><i class="fa fa-plus" aria-hidden="true"></i> <u>A</u>dd</a></th>
      <th><a href="?period='.$period.'&action=xlse"><i class="far fa-file-excel fa-lg" aria-hidden="true"></i>XLS</a></th>';
?>
  </tr>
  <tr valign=top>
     <th>Day</th>
     <th>Name - Description</th>
     <th>Still to pay</th>
     <th>Amount Paid</th>
     <th>Method<div class="pure-form">
              <select onchange="changePeriodMethod(this.value)">
                      <option value='?'>All</option>
     <?php

              $selM[$_GET['method']] = 'selected';

              echo "<option value='method=0' ".$selM[0].">Cash</option>";
              echo "<option value='method=2' ".$selM[2].">Bank</option>";
              echo "<option value='method=3' ".$selM[3].">Paypal</option>";

     ?>

              </select></div></th>
     <th>Personel</th>
     <th>Location</th>
     <th></th>
     <th></th>
  </tr>
 </thead>

<?php


if($_GET['method'] <> '')
  $method_sel = 'and cash_book.type_payment ='.$_GET['method'];
else
  $method_sel = '';

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
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                              cash_book.scan_org_filename_ext,
                              cash_book.type_payment,

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
                              ".$method_sel."
                              AND MONTH(`cash_book`.`date`) = '".$period_month."'
                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              ORDER BY cash_book.date desc");

$stmt -> execute();


$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   $route_txt = '';
   if($row['boat_trip_id']>0)
      $route_txt = '<br><font style="font-size:70%;" color=grey>'.$row['date_from'].' - '.$row['date_to'].': '.$row['txt_route'].'</font>';

   if($row['value']==0)
      $value_error_color = ' style="background-color:#FFCCCB"';
   else
      $value_error_color = '';      

   if($row['type_payment'] == 0) $payment_type_txt='Cash';
   if($row['type_payment'] == 2) $payment_type_txt='Bank';
   if($row['type_payment'] == 3) $payment_type_txt='Paypal';


   echo '<tr>
   <td align=center>'.$row['dzien'].'</td>
   <td>'.$row['cb_name'].' - '.$row['desc'].$route_txt.'</td>';

   if($row['costs_bill_value']<>0 and ($row['value']-$row['costs_bill_value'])<>0)
      {
       $still_to_pay = $formatter->formatCurrency($row['costs_bill_value']-$row['value'], 'IDR');
       $sum_still += $row['costs_bill_value']-$row['value'];
       }
   else
       $still_to_pay = '';

   echo '
   <td align=right>'.$still_to_pay.'</td>';

   echo '
   <td align=right'.$value_error_color.'>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>
   <td><font style="font-size:80%;">'.$payment_type_txt.'</font></td>
   <td><font style="font-size:80%;">'.$row['pers_name'].'</font></td>
   <td><font style="font-size:80%;">'.$row['loc_name'].'</font></td>
   <td>';
   
   if(file_exists('img_uploads/costs/costs_'.$row['id_cash_book'].'.'.$row['scan_org_filename_ext']))
            echo '<a href="img_uploads/costs/costs_'.$row['id_cash_book'].'.'.$row['scan_org_filename_ext'].'" target=_blank><i class="far fa-file-alt"></i></a>';
   
   echo '</td>
   <td>';

    if($_SESSION['permission']=='rw')
       {
        

        echo '<a href="?action=edit&id='.$row['id_cash_book'].'"><i class="far fa-edit" aria-hidden="true"></i>[e]</a>';
       }

   echo '
   </td>
   </tr>';

   $sum_value += $row['value'];
   }


?>

<tr>
 <td colspan=2></td>
 <td class=cash><?php echo $formatter->formatCurrency($sum_still, 'IDR'); ?></td>
 <td class=cash><?php echo $formatter->formatCurrency($sum_value, 'IDR'); ?></td>
 <td colspan=4></td>
</tr>

</table>
