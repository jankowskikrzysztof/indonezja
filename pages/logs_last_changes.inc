<script>
function changePeriod(url)
{
    location.href = 'logs.php?action=last_changes&table=cash_book&period=' + url;
}
</script>


<div align=center class="pure-form">

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>


<?php

$table = $_GET['table'];

$select = "SELECT *
                              FROM `log_".$table."`
                              WHERE action<>'insert'
                              and YEAR(dt_datetime)=".$period_year."
                              and MONTH(dt_datetime)=".$period_month."
                              ORDER BY dt_datetime desc";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();


echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr class=reports  align=center>';

for($i = 0; $i<$stmt->columnCount(); $i++)
   {
   $col = $stmt->getColumnMeta($i);

   if($col['name'] == 'id_cash_book')
   echo '<th>Operator/IP</th>';


   echo '<th>'.$col['name'].'</th>';
   }

echo '</tr>
 </thead>';


$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr class=reports>';

   for($x=0; $x<$stmt->columnCount(); $x++)
      {
      $col = $stmt->getColumnMeta($x);


      if($col['name'] == 'dt_datetime')
        $dataczas = $row[$x];
      if($col['name'] == 'id_cash_book')
        {
        $target_id = $row[$x];

        $stmt2 = $dbh->prepare("SELECT logs.*, personel.login
                              FROM `logs`
                              left join personel on logs.personel_id=personel.id_personel
                              WHERE target_id= ".$target_id."
                              and target_table='cash_book'
                              and insertdate='".$dataczas."'
                              ");

        $stmt2 -> execute();
        $edit_array = $stmt2->fetchAll();

        $logs_author = $edit_array[0][login].'<br>ip:'.$edit_array[0][ip];

        echo '<td align=center>'.$logs_author.'</td>';
        }




      $tablica_kwot = array('value','costs_bill_value','value_before_discount','discount');

      if($col['name'] == 'id_cash_book')
         $txt_col = '<a href="logs.php?action=table&table=cash_book&id='.$row[$x].'">'.$row[$x].'</a>';
      elseif(in_array($col['name'],$tablica_kwot))
         $txt_col = $formatter->formatCurrency($row[$x], 'IDR');
      else
         $txt_col = $row[$x];

      echo '<td align=center>'.$txt_col.'</font></td>';

      }

   echo '</tr>';
   }

?>
</table>
