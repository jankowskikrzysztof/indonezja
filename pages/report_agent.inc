<script>
function changePeriod(url)
{
    location.href = 'report.php?action=agent&period=' + url;
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
                              agent.name as agent_name,
                              cash_book.agent_commission,
                              cash_book.agent_commission_currency,
                              cash_book.agent_perc,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name

                              FROM cash_book, personel, location, cash_book_group, agent
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              and cash_book.agent_id=agent.id_agent
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
     <td>'.$row['date'].'</td>
     <td><a href="income.php?action=edit&id='.$row['id_cash_book'].'">'.$row['bill_no'].'</a></td>
     <td>'.$row['loc_name'].'</td>
     <td>'.$row['agent_name'].'</td>
     <td>'.$row['agent_perc'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['agent_commission'], 'IDR').'</td>';
     //echo '<td class=cash>'.$formatter->formatCurrency($row['agent_commission_currency'], 'IDR').'</td>';


     echo '</tr>';

  $sum_agent_commission += $row['agent_commission'];
  $sum_agent_commission_currency += $row['agent_commission_currency'];

  }


?>




</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>Bill No</th>
     <th>Location</th>
     <th>Agent</th>
     <th>%</th>
     <th>Commission</th>
     <th></th>
  </tr>
 </thead>
<tfoot>
    <tr align=center valign=top>
     <td colspan=5></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_agent_commission, 'IDR'); ?></td>
     <td class=cash><?php //echo $formatter->formatCurrency($sum_agent_commission_currency, 'IDR'); ?></td>
  </tr>
</tfoot>
</table>
</div>
