<script>
function changePeriod(url)
{
    location.href = 'report_group_settlements.php?action=cc&period=' + url;
}
</script>

<script>
function changePeriodLoc(url)
{
    location.href = 'report_group_settlements.php?action=cc&period=<?php echo $_GET['period'];?>&' + url;
}
</script>


<?php
$stmt = $dbh->prepare("SELECT location.id_location, location.name as location_name
                              FROM location

                              ORDER BY location.name");
$stmt -> execute();
$location_array = $stmt->fetchAll();
?>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

<div>
  <table class="pure-table pure-table-horizontal pure-table-striped">
 <tbody>


<?php




if($_SESSION['location_id_limit']<>0)
   {
   $loc_sel = $_SESSION['location_id_limit'];
   $where_limit = 'and location.id_location IN ('.$loc_sel.')';
   }

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';

if($_GET['loc_sel'] <> '')
      {
      $location_key = array_search($_GET['loc_sel'], array_column($booker_array, 'id_location'));
      
      $location_statement_filter = 'and description LIKE "%'.strtoupper($booker_array[$booker_key]['location_name']).'%"'; // poprawic
      $location_sel = 'and location.id_location ='.$_GET['loc_sel'];
      }
else
      {
      $location_statement_filter = '';
      $location_sel = '';
      }

//echo $sel_loc_pers.'a<br>';

$select = "SELECT cash_book_group.name as cb_name, location.id_location, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc,
activity.name as act_name, activity.shortcut as act_shortcut, cash_book.id_cash_book, cash_book.date, cash_book.bill_no, cash_book.currency,
location_groups.name as loc_grp_name, DAY(cash_book.date) as dzien,

cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,
cash_book.boat_trip_id,


cash_book.value,
GROUP_CONCAT(activity.shortcut) as shortcut

FROM cash_book, cash_book_item, personel, location, cash_book_group, activity, location_groups
WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
and cash_book.personel_id=personel.id_personel
and cash_book.type=1
and cash_book.cash_report_only=0

                              ".$sel_loc_pers."
                  			      ".$where_limit."
                              ".$period_limit."
                              ".$location_sel."
and cash_book.pay_creditcard<>0

AND MONTH(`cash_book`.`date`) = '".$period_month."'
AND YEAR(`cash_book`.`date`) = '".$period_year."'
and activity.id_activity=cash_book_item.activity_id
and cash_book_item.cash_book_id=cash_book.id_cash_book
and location.id_location=cash_book.location_id
and location.location_groups_id=location_groups.id_location_groups
GROUP BY cash_book.id_cash_book
ORDER BY cash_book.date desc";


//                              AND MONTH(`cash_book`.`date`) = '".$period_month."'
//                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

    $payment_type = '';
    if($row['pay_cash']<>0)
       $payment_type .= 'cash,';
    if($row['pay_creditcard']<>0)
       $payment_type .= 'creditcard,';
    if($row['pay_roomcharge']<>0)
       $payment_type .= 'roomcharge,';
    if($row['pay_bank']<>0)
       $payment_type .= 'bank,';
    if($row['pay_paypal']<>0)
       $payment_type .= 'paypal,';

     echo '<tr>
     <td>'.$row['date'].', <font size=0.5vw>'.$row['id_cash_book'].'</font></td>
     <td>'.$row['loc_name'].'</td>
     <td>'.$row['bill_no'].'</td>
     <td>'.$row['cb_name'].'</td>
     <td>'.$row['desc'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['pay_creditcard'], 'IDR').'</td>';

     echo '</tr>';

  $sum_cc += $row['pay_creditcard'];

  }


?>

  <tr align=center valign=top>
     <td colspan=5></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_cc, 'IDR'); ?></td>
  </tr>


</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>Location<div class="pure-form">
              <select onchange="changePeriodLoc(this.value)">
                      <option value='?'>All</option>
     <?php
           foreach($location_array as $row)
              {
              if($row['id_location'] == $_GET['loc_sel'])
                 $sel = 'selected';
              else
                 $sel = '';

              echo "<option value='loc_sel=".$row['id_location']."' ".$sel.">".$row['location_name']."</option>";

              }
     ?>

              </select></div></th>
     <th>Bill No</th>
     <th>Group</th>
     <th>Description</th>
     <th>Pay CC</th>
  </tr>
 </thead>

</table>
</div>

<br><br>


<div>
  <table class="pure-table pure-table-horizontal pure-table-striped">
 <tbody>


<?php



$select = "SELECT bank_statement.*
FROM bank_statement
WHERE bank_statement.type='TRANSFER DARI'
and (description LIKE ' PEMINDAHAN DARI 774360289411003   SCUBA FROGGY MBL   OTOPAY KK%'
or description LIKE '%GIRO INTERNAL%'
or description LIKE ' PEMINDAHAN DARI 774360289411003   SCUBA FROGGY MBL   OTOPAY HLD%'
or description LIKE ' PEMINDAHAN DARI 774360289411003   SCUBA FROGGY MBL   PEMBAYARAN%')


and MONTH(`value_date`) = '".$period_month."' and YEAR(`value_date`) = '".$period_year."'


ORDER BY bank_statement.value_date DESC";


//and value_date between DATE_ADD('".$period_year."-".$period_month."-01', INTERVAL 1 DAY) and DATE_ADD(LAST_DAY('".$period_year."-".$period_month."-01'), INTERVAL 1 DAY)


//and MONTH(`value_date`) = '".$period_month."' and YEAR(`value_date`) = '".$period_year."'
//


//                              AND MONTH(`cash_book`.`date`) = 
//                              AND YEAR(`cash_book`.`date`) = 
//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {


     echo '<tr>
     <td>'.$row['value_date'].', <font size=0.5vw>'.$row['id_bank_statement'].'</font></td>
     <td>'.$row['description'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['credit'], 'IDR').'</td>';

     echo '</tr>';

  $sum_in += $row['credit'];

  }


?>

  <tr align=center valign=top>
     <td colspan=2></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_in, 'IDR'); ?></td>
  </tr>


</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>description</th>

     <th>IN</th>
  </tr>
 </thead>
</table>
</div>

