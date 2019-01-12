<script>
function changePeriod(url)
{
    location.href = 'report_monthly_room_charge.php?period=' + url;
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



/*



*/




if($_SESSION['location_id_limit']<>0)
   {
   $loc_sel = $_SESSION['location_id_limit'];
   $where_limit = 'and location.id_location IN ('.$loc_sel.')';
   }

$stmt = $dbh->prepare("SELECT
  SUM(`cash_book`.`value`) AS value,
  location.room_charge_perc,
  SUM(`cash_book`.`value`)*location.room_charge_perc/100 as room_charge_value,
  `location_groups`.`name` AS group_name,
  `location`.`name` AS loc_name,
  `cash_book`.`type`,
  case cash_book.type
       when '1' then 'income'
       when '2' then 'cost'
  end as type_name

FROM
  `cash_book`,
  `location`,
  `location_groups`
WHERE
     `cash_book`.`location_id` = `location`.`id_location`
  AND `location`.`location_groups_id` = `location_groups`.`id_location_groups`
  AND MONTH(`cash_book`.`date`) = '".$period_month."'
  AND YEAR(`cash_book`.`date`) = '".$period_year."'
  and `cash_book`.`type`=1
  and location.room_charge_perc<>0
  ".$where_limit."
GROUP BY
  `cash_book`.`type`, location_groups.id_location_groups
ORDER BY
  `location`.`name`");

$stmt -> execute();

$row_array = $stmt->fetchAll();


foreach($row_array as $row)
   {

     echo '<tr>
     <td>'.$row['loc_name'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').' IDR</td>';
     echo '<td class=cash>'.$formatter->formatCurrency($row["room_charge_value"], 'IDR').' IDR</td>';
     echo '</tr>';

  }


?>
</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Location</th>
     <th>Result</th>
     <th>Hotel</th>
  </tr>
 </thead>
<tfoot>
    <tr>
      <td colspan=3 align=right></td>
    </tr>
</tfoot>
</table>
</div>
