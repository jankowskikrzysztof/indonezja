<script>
function changePeriod(url)
{
    location.href = 'bank_statement.php?period=' + url;
}
</script>


<div align=center>

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>

  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr align=center>
     <th>Account NO</th>
     <th>Post Date</th>
     <th>Description</th>
     <th>Recognized</th>
     <th>Credit/IN</th>
     <th>Debit/OUT</th>
  </tr>
 </thead>

<?php

$select = "SELECT account_no,post_date,value_date,branch,journal_no,description,debit,credit
                              FROM bank_statement

                              WHERE MONTH(`post_date`) = '".$period_month."'
                              AND YEAR(`post_date`) = '".$period_year."'
                              ORDER BY account_no, post_date asc";
//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   if($row['account_no']=='380251201')
     $curr = 'USD';
   if($row['account_no']=='359738937')
     $curr = 'IDR';

   if($last_curr<>$curr)
     echo '<tr bgcolor="#A8D08D"><td colspan=6>&nbsp;</td></tr>';

   $description = str_replace('|','<br>', str_replace(array('TRANSFER DARI | PEMINDAHAN DARI','TRANSFER KE | PEMINDAHAN KE'),'', $row['description']) );

   //$recognized = substr($description,0,10);
   $recognized = '';

   if(substr($description,0,10) == '380251201')
      $recognized = 'internal transf';
   if(substr($description,0,12) == '359738937')
      $recognized = 'internal transf';


   echo '<tr>
   <td>'.$row['account_no'].'</td>
   <td>'.$row['post_date'].'</td>
   <td>'. $description.'</td>
   <td>'. $recognized.'</td>
   <td align=right>'; if($row['credit']<>0) echo $formatter->formatCurrency($row['credit'], $curr).' '.$curr;
   echo '</td>
   <td align=right>'; if($row['debit']<>0) echo $formatter->formatCurrency($row['debit'], $curr).' '.$curr;
   echo '</td>
   </tr>';

   $sum_debit += $row['debit'];
   $sum_credit += $row['credit'];
   $last_curr = $curr;
   }


?>
<!--
<tr>
 <td colspan=5></td>
 <td class=cash><?php echo $formatter->formatCurrency($sum_still, 'IDR'); ?></td>
 <td class=cash><?php echo $formatter->formatCurrency($sum_value, 'IDR'); ?></td>
 <td></td>
</tr>
-->

</table>
