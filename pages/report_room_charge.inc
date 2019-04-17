<script>
function changePeriod(url)
{
    location.href = 'report.php?action=room_charge&period=' + url;
}
</script>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<a href="?action=room_charge&period=<?php echo $period_year.'-'.$period_month.'';?>&yr=0">month</a> | <a href="?action=room_charge&period=<?php echo $period_year.'-'.$period_month.'';?>&yr=1">year</a>
<br><br>
</div>

<h2 align=center>Room charge SUM</h2>

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
   {
     $period_limit_bs = 'and value_date >= now() - interval '.$_SESSION['limit_data_months'].' month';
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
   }
else
   {
    $period_limit_bs = '';
      $period_limit = '';
   }

if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_GET['yr']=='1')
   $sel_month = '';
else
   $sel_month = 'and MONTH(cash_book.date)="'.$period_month.'"';


$select = "
    SELECT 
           location.name as loc_name, 
           location.room_charge_perc, 
           100-location.room_charge_perc as scuba_perc, 
           SUM(cash_book.value) as value, 
           SUM(cash_book.value)*location.room_charge_perc/100 as hotel_amount, 
           SUM(cash_book.value) - SUM(cash_book.value)*location.room_charge_perc/100 as scuba_amount 
           
        FROM `cash_book`,`location` 
        WHERE cash_book.location_id=location.id_location 
          and location.room_charge_perc<>0
          and cash_book.type=1
          and cash_book.cash_report_only<>1
    
          and YEAR(cash_book.date)='".$period_year."'
          ".$sel_month."
          ".$where_limit."
          ".$period_limit."
          ".$sel_loc_pers."
        GROUP BY cash_book.location_id
        ORDER BY location.name
    ";
    
    //echo $select;
    
    $stmt = $dbh->prepare($select);
    $stmt -> execute();
    
    $row_array = $stmt->fetchAll();
    
    foreach($row_array as $row)
       {
        echo '<tr>
        <td>'.$row['loc_name'].'</td>
        <td align=center>'.$row['room_charge_perc'].' %</td>
        <td align=center>'.$row['scuba_perc'].' %</td>
        ';
   
        echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';
        echo '<td class=cash>'.$formatter->formatCurrency($row['hotel_amount'], 'IDR').'</td>';
        echo '<td class=cash>'.$formatter->formatCurrency($row['scuba_amount'], 'IDR').'</td>';
   
        echo '</tr>';
   
     $sum_value += $row['value'];
     $sum_hotel_amount += $row['hotel_amount'];
     $sum_scuba_amount += $row['scuba_amount'];
   
     }
   
   echo '<tfoot>
     <tr>
       <td colspan=3></td>
       <td class=cash>'.$formatter->formatCurrency($sum_value, 'IDR').'</td>
       <td class=cash>'.$formatter->formatCurrency($sum_hotel_amount, 'IDR').'</td>
       <td class=cash>'.$formatter->formatCurrency($sum_scuba_amount, 'IDR').'</td>
     </tr>
     </tfoot>
   
   
   
   '
   
   
   ?>
   
     <thead>
     <tr align=center valign=top>
        <th>Location</th>
        <th>Room Charge %</th>
        <th>Scuba %</th>
        <th>Bill Value</th>
        <th>Hotel Part</th>
        <th>Scuba Part</th>
     </tr>
    </thead>
   
   </table>
   </div>  

<br><br>
<h2 align=center>Room charge detailed</h2>

<div>
  <table class="pure-table pure-table-horizontal pure-table-striped">
 <tbody>

<?php

$sum_value = 0;
$sum_hotel_amount = 0;
$sum_scuba_amount = 0;


$select = "
SELECT cash_book.id_cash_book, 
       location.name as loc_name, 
       cash_book.date, 
       location.room_charge_perc, 
       100-location.room_charge_perc as scuba_perc, 
       cash_book.value, 
       cash_book.value*location.room_charge_perc/100 as hotel_amount, 
       cash_book.value - cash_book.value*location.room_charge_perc/100 as scuba_amount 
       
    FROM `cash_book`,`location` 
    WHERE cash_book.location_id=location.id_location 
      and location.room_charge_perc<>0
      and cash_book.type=1
      and cash_book.cash_report_only<>1

      and YEAR(cash_book.date)='".$period_year."'
      ".$sel_month."
      ".$where_limit."
      ".$period_limit."
      ".$sel_loc_pers."
    ORDER BY cash_book.date desc
";

//echo $select;

$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {


     echo '<tr>
     <td><a href="income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['date'].'</a></td>
     <td>'.$row['loc_name'].'</td>
     <td align=center>'.$row['room_charge_perc'].' %</td>
     <td align=center>'.$row['scuba_perc'].' %</td>
     ';

     echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row['hotel_amount'], 'IDR').'</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row['scuba_amount'], 'IDR').'</td>';

     echo '</tr>';

  $sum_value += $row['value'];
  $sum_hotel_amount += $row['hotel_amount'];
  $sum_scuba_amount += $row['scuba_amount'];

  }

echo '<tfoot>
  <tr>
    <td colspan=4></td>
    <td class=cash>'.$formatter->formatCurrency($sum_value, 'IDR').'</td>
    <td class=cash>'.$formatter->formatCurrency($sum_hotel_amount, 'IDR').'</td>
    <td class=cash>'.$formatter->formatCurrency($sum_scuba_amount, 'IDR').'</td>
  </tr>
  </tfoot>



'


?>

  <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>Location</th>
     <th>Room Charge %</th>
     <th>Scuba %</th>
     <th>Bill Value</th>
     <th>Hotel Part</th>
     <th>Scuba Part</th>
  </tr>
 </thead>

</table>
</div>


<br><br>
<h2 align=center>Bank Statement and Cash Book records (year)</h2>


<div>
  <table class="pure-table pure-table-horizontal pure-table-striped">
 <tbody>


<?php



$select = "SELECT bank_statement.value_date as date,
bank_statement.id_bank_statement as id,
bank_statement.description,
'bank' as value_source,
bank_statement.credit as cash

FROM bank_statement
WHERE (
     description LIKE '%DARI NOVOTEL%'
  or description LIKE '%MANDALIKA%'
  or description LIKE '%ISTANA%'
  or description LIKE '%QUEEN GILI TRAW%'
  
)
and account_no = '359738937'
and YEAR(`value_date`) = '".$period_year."'
$period_limit_bs


UNION

SELECT cash_book.date,
cash_book.id_cash_book as id,
cash_book.desc as description,
'cash' as value_source,
cash_book.pay_cash as cash

FROM `cash_book` 
WHERE `cash_book_group_id` = 55
and cash_book.type=1
and cash_report_only=1
and YEAR(`date`) = '".$period_year."'
$period_limit
$where_limit 

ORDER BY date DESC";


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
     <td>'.$row['date'].', <font size=0.5vw>'.$row['id'].'</font></td>
     <td>'.$row['value_source'].'</td>
     <td>'.$row['description'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['cash'], 'IDR').'</td>';

     echo '</tr>';

  $sum_in += $row['cash'];

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
     <th>Type</th>
     <th>Description</th>

     <th>IN</th>
  </tr>
 </thead>
</table>
</div>
