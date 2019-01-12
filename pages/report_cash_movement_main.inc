<script>
function changePeriod(url)
{
    location.href = 'report_cash_movement.php?period=' + url;
}
</script>


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


//echo $sel_loc_pers.'a<br>';

$select = "SELECT location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date, cash_book.pay_cash, cash_book.cash_report_only,
                              cash_book_group.name as cb_name,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name

                              FROM cash_book, personel, location, cash_book_group
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.cash_book_group_id IN (21,22,23,24)
                              ".$sel_loc_pers."
                  			      ".$where_limit."
                              ".$period_limit."
                              AND MONTH(`cash_book`.`date`) = '".$period_month."'
                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              and location.id_location=cash_book.location_id
                              and ( (cash_book.pay_cash<>0 and cash_book.type=1) or (cash_book.value<>0 and cash_book.type=2) )

                              ORDER BY cash_book.date desc, cash_book.value+cash_book.pay_cash";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

     echo '<tr>
     <td>'.$row['date'].', <font size=0.5vw>'.$row['id_cash_book'].'</font></td>
     <td>'.$row['loc_name'].'</td>
     <td>'.$row['pers_name'].'</td>
     <td>'.$row['cb_name'].'</td>
     <td>'.$row['desc'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row[pay_cash], 'IDR').'</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row[value], 'IDR').'</td>';

    echo '<td>';

                  $stmt2 = $dbh->prepare("SELECT settlements.*, bank_statement.*
                  FROM settlements, bank_statement
                  WHERE settlements.foreign_id=".$row['id_cash_book']."
                  and settlements.bank_statement_id=bank_statement.id_bank_statement
                  and settlements.foreign_table='cash_book'
                  ORDER BY bank_statement.value_date DESC
                  ");
                  $stmt2 -> execute();
                  $last_change = $stmt2->fetchAll();  
                  //$sum_settlement = 0;
                  foreach($last_change as $row1)
                          {
                          //echo '<tr><td>'.$row['value_date'].'</td><td>'.$row['account_no'].'</td><td>'.$row['value'].'</td></tr>';
                          echo $row1['value_date'].', '.$row1['account_no'].': '.$formatter->formatCurrency($row1['value'], 'IDR');

                          $sum_settlement += $row1['value'];
                          }

    echo '</td>';

     echo '</tr>';

  $sum_in += $row[pay_cash];
  $sum_out += $row[value];

  }


?>

  <tr align=center valign=top>
     <td colspan=5></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_in, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_out, 'IDR'); ?></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_settlement, 'IDR'); ?></td>
  </tr>


</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>Location</th>
     <th>Pesonel</th>
     <th>Group</th>
     <th>Description</th>
     <th>IN</th>
     <th>OUT</th>
     <th>Settlement</th>
  </tr>
 </thead>
<tfoot>
    <tr>
      <td colspan=7 align=right></td>
    </tr>
</tfoot>
</table>
</div>
