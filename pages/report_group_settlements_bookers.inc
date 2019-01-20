<script>
function changePeriod(url)
{
    location.href = 'report_group_settlements.php?action=bookers&period=' + url;
}
</script>

<script>
function changePeriodBook(url)
{
    location.href = 'report_group_settlements.php?action=bookers&period=<?php echo $_GET['period'];?>&' + url;
}
</script>

<?php
$stmt = $dbh->prepare("SELECT booker.id_booker, booker.name as book_name
                              FROM booker

                              ORDER BY booker.name");
$stmt -> execute();
$booker_array = $stmt->fetchAll();
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

if($_GET['book_sel'] <> '')
   {
   echo '<pre>';
   var_dump($booker_array);
   echo '</pre>';


   $booker_statement_filter = '';
   $book_sel = 'and booker.id_booker ='.$_GET['book_sel'];
   }
else
   {
   $booker_statement_filter = '';
   $book_sel = '';
   }

//echo $sel_loc_pers.'a<br>';

$select = "SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc,
activity.name as act_name, activity.shortcut as act_shortcut, cash_book.id_cash_book, cash_book.date, cash_book.bill_no, cash_book.currency,
location_groups.name as loc_grp_name, DAY(cash_book.date) as dzien,

cash_book.pay_cash,cash_book.pay_creditcard,cash_book.pay_roomcharge,cash_book.pay_bank,cash_book.pay_paypal,
cash_book.pay_cash+cash_book.pay_creditcard+cash_book.pay_roomcharge+cash_book.pay_bank+cash_book.pay_paypal as total_pay,
cash_book.boat_trip_id,

booker.name as booker_name,

cash_book.value,
cash_book.value_currency,
GROUP_CONCAT(activity.shortcut) as shortcut

FROM (cash_book, cash_book_item, personel, location, cash_book_group, activity, location_groups)
left join booker on cash_book.booker_id = booker.id_booker
WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
and cash_book.personel_id=personel.id_personel
and cash_book.type=1
and cash_book.cash_report_only=0

                              ".$sel_loc_pers."
                  			   ".$where_limit."
                              ".$period_limit."
                              ".$book_sel."

and cash_book.booker_id<>0

AND YEAR(`cash_book`.`date`) = '".$period_year."'
and activity.id_activity=cash_book_item.activity_id
and cash_book_item.cash_book_id=cash_book.id_cash_book
and location.id_location=cash_book.location_id
and location.location_groups_id=location_groups.id_location_groups
GROUP BY cash_book.id_cash_book
ORDER BY cash_book.date desc";

//AND MONTH(`cash_book`.`date`) = '".$period_month."'

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
     <td>'.$row['desc'].'</td>
     <td>'.$row['booker_name'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['value_currency'], 'USD').'</td>';

     echo '</tr>';

  $sum_bookers += $row['value_currency'];

  }


?>

  <tr align=center valign=top>
     <td colspan=6></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_bookers, 'USD'); ?></td>
  </tr>


</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>Location</th>
     <th>Bill No</th>
     <th>Group</th>
     <th>Description</th>
     <th>Booker<div class="pure-form">
              <select onchange="changePeriodBook(this.value)">
                      <option value='?'>All</option>
     <?php
           foreach($booker_array as $row)
              {
              if($row['id_booker'] == $_GET['book_sel'])
                 $sel = 'selected';
              else
                 $sel = '';

              echo "<option value='book_sel=".$row['id_booker']."' ".$sel.">".$row['book_name']."</option>";

              }
     ?>

              </select></div></th>
     <th>Payment</th>
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
WHERE (bank_statement.type='TRANSFER DARI' or bank_statement.type='KREDIT')
and (
     description LIKE '%LIVEABOARD.COM%'
  or description LIKE '%PADI TRAVEL%'
  or description LIKE '%DIVEBOOKER.COM%'
  or description LIKE '%DIVIAC%'
  or description LIKE '%DIVE THE WORLD%'
)
and account_no = '380251201'
and YEAR(`value_date`) = '".$period_year."'
".$booker_statement_filter."

ORDER BY bank_statement.value_date DESC";


//and MONTH(`value_date`) = '".$period_month."'


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

