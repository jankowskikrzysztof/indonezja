<script>
function changePeriod(url)
{
    location.href = 'costs.php?action=without_trips&period=' + url;
}
</script>


<div align=center class="pure-form" >

<h1>Empress costs without Trips (except Salaries)</h1>

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


$select = "


SELECT cash_book_group.name as cb_name, location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
                              cash_book.id_cash_book, cash_book.date,cash_book.costs_bill_value, DAY(cash_book.date) as dzien,

                              boat_trip.date_from, boat_trip.date_to, boat_trip.txt_route, cash_book.boat_trip_id,

                              
                              cash_book.type_payment,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name,
                                  
                                 cash_book.location_id
                                  
                              FROM (personel, location, cash_book_group,cash_book)
                              left join boat_trip on boat_trip.id_boat_trip=cash_book.boat_trip_id
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              
                              and cash_book.location_id=14
                              and cash_book.cash_book_group_id<>4

                              and cash_book.boat_trip_id=0
                              
                              ".$sel_loc_pers."
                  			      ".$where_limit."
                              ".$period_limit."


                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              and cash_book.type=2
                              and cash_book.cash_report_only=0
                              and location.id_location=cash_book.location_id
                              ORDER BY cash_book.date desc";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {

     echo '<tr>
     <td><a href="costs.php?action=edit&id='.$row['id_cash_book'].'">'.$row['date'].'</a></td>
     <td>'.$row['cb_name'].'</td>
     <td>'.$row['desc'].'</td>';

     echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';

     echo '</tr>';

  $sum_value += $row['value'];

  }


?>




</tbody>
   <thead>
  <tr align=center valign=top>
     <th>Date</th>
     <th>Name</th>
     <th>Description</th>
     <th>Value</th>
  </tr>
 </thead>
<tfoot>
    <tr align=center valign=top>
     <td colspan=3></td>
     <td class=cash><?php echo $formatter->formatCurrency($sum_value, 'IDR'); ?></td>
  </tr>
</tfoot>
</table>
</div>
