<script>
function changePeriod(url)
    {
    location.href = 'cash_report.php?period=' + url;
    }
</script>


<div align=center class="pure-form">

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>

<?php



if($personel_location_selected>0)
  {

?>



</div>

  <table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
  <tr>

<?php
if($_SESSION['permission']=='rw')
echo '  
    <th colspan=2 align=right><a class="pure-button pure-button-primary" href="?action=add&cash=in" accesskey="i"><i class="fa fa-plus" aria-hidden="true"></i> Cash <u>I</u>n</a></th>
      <th colspan=6 align=left><a class="pure-button pure-button-primary" href="?action=add&cash=out" accesskey="o"><i class="fa fa-plus" aria-hidden="true"></i> Cash <u>O</u>ut</a></th>';
?>
  </tr>
  <tr>
     <th>Day</th>
<!--     <th>Location</th> -->
<!--     <th>Payment Type</th> -->
     <th>Group - Description</th>
     <th align=center>In</th>
     <th align=center>Out</th>
     <th align=center>Cash position</th>
     <th>Personel</th>
     <th></th>
     <th></th>
  </tr>
 </thead>

<?php

if($personel_location_selected <> '')
    $sel_loc_pers = 'and location.id_location IN ('.$personel_location_selected.')';
else
    $sel_loc_pers = '';

if($_SESSION['limit_data_months']<>0)
      $period_limit = 'and cash_book.date >=  now() - interval '.$_SESSION['limit_data_months'].' month';
else
      $period_limit = '';



$select_last_position = "SELECT
sum(case when cash_book.type=1 then cash_book.pay_cash else 0 end) as cash_in,
sum(case when cash_book.type=2 then cash_book.value else 0 end) as cash_out


FROM cash_book, personel, location, cash_book_group
WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
and cash_book.personel_id=personel.id_personel
".$sel_loc_pers."
AND `cash_book`.`date` < '".$period_year."-".$period_month."-01'
and location.id_location=cash_book.location_id
and ( (cash_book.pay_cash<>0 and cash_book.type=1) or (cash_book.value<>0 and cash_book.type=2) )
and cash_book.type_payment=0

GROUP BY location.id_location
ORDER BY cash_book.date";

$stmt = $dbh->prepare($select_last_position);
$stmt -> execute();
$row_array = $stmt->fetchAll();
foreach($row_array as $row)
   {
   $last_cash_in = $row['cash_in'];
   $last_cash_out = $row['cash_out'];
   $last_cash_position = $last_cash_in - $last_cash_out;
   }


echo '<tr>
   <td colspan=4 align=right>Balance from last month -></td>
   <td align=right>'.$formatter->formatCurrency($last_cash_position, 'IDR').'</td>
   <td></td>
   <td></td>
   <td></td>
   </tr>';

$cash_position = $last_cash_position;


$select = "SELECT location.name as loc_name, personel.name as pers_name, cash_book.type, cash_book.desc, cash_book.value, cash_book.currency,
			      cash_book.cash_book_source,
                              cash_book.id_cash_book, cash_book.date, cash_book.pay_cash, cash_book.cash_report_only,
                              cash_book_group.name as cb_name, DAY(cash_book.date) as dzien,
                              GROUP_CONCAT(settlements.id_settlements) as id_settlements,

                                  case cash_book.type
                                     when '1' then 'income'
                                     when '2' then 'cost'
                                  end as type_name

                              FROM (cash_book, personel, location, cash_book_group)
                              left join settlements on settlements.foreign_id=cash_book.id_cash_book and settlements.foreign_table='cash_book'
                              WHERE cash_book_group.id_cash_book_group=cash_book.cash_book_group_id
                              and cash_book.personel_id=personel.id_personel
                              ".$sel_loc_pers."
                              ".$period_limit."
                              AND MONTH(`cash_book`.`date`) = '".$period_month."'
                              AND YEAR(`cash_book`.`date`) = '".$period_year."'
                              and location.id_location=cash_book.location_id
                              and ( (cash_book.pay_cash<>0 and cash_book.type=1) or (cash_book.value<>0 and cash_book.type=2) )
                              and cash_book.type_payment=0

                              GROUP BY cash_book.id_cash_book

                              ORDER BY cash_book.date, cash_book.type
                              
                              ";

//echo $select;
$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();

foreach($row_array as $row)
   {
   echo '<tr>
   <td align=center>'.$row['dzien'].'</td>';

   //<!--<td>'.$row['loc_name'].'</td>-->


   //<!--<td>'.$row['type_name'].'</td>-->

   echo '<td class=reports>'.$row['cb_name'].' - '.$row['desc'].'</td>';

   $cash_in =0;
   $cash_out =0;

   if($row['type'] == 1)
      {
      echo '<td align=right>'.$formatter->formatCurrency($row['pay_cash'], 'IDR').'</td><td></td>';
      $sum_in += $row['pay_cash'];
      $cash_in = $row['pay_cash'];
      }

   if($row['type'] == 2)
      {
      echo '<td></td><td align=right>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';
      $sum_out += $row['value'];
      $cash_out = $row['value'];
      }

   $cash_position = $cash_position + $cash_in - $cash_out;

   if($cash_position <0)
      $font_cash_pos = '<font color=red>';
   else
      $font_cash_pos = '<font>';


   echo '<td align=right>'.$font_cash_pos.$formatter->formatCurrency($cash_position, 'IDR').'</font></td>';

   if($row['cash_book_source']<>0)
	{
	$bg_pers = '<font style="font-size:80%;" color=red>';
	}
   else
	{
	$bg_pers = '<font style="font-size:80%;">';
	}

   echo '<td>'.$bg_pers.''.$row['pers_name'].'</font></td>


        <td>';

   if($row['cash_report_only'] <> 0)
       {
       if($row['type']==1) $cash_type = '&cash=in';
       if($row['type']==2) $cash_type = '&cash=out';

       if(($row['cash_book_source']==0 and $row['type']==1) or $row['type']==2)
	 if($_SESSION['permission']=='rw') 
	     echo '<a href="?action=edit&id='.$row['id_cash_book'].$cash_type.'"><i class="far fa-edit" aria-hidden="true"></i>[e]</a>';
       }

   echo '</td>
         <td>';


   if($_SESSION['login'] == 'kjankowski' or $_SESSION['login'] == 'przemek' or $_SESSION['login'] == 'maciek' or $_SESSION['login'] == 'marcin')
       {
       $stmt = $dbh->prepare("SELECT COUNT(id_cash_book) as count
                              FROM `log_cash_book`
                              WHERE id_cash_book= ".$row['id_cash_book']."
                              GROUP BY id_cash_book
                              ");


       $stmt -> execute();
       $edit_array = $stmt->fetchAll();

       $count_logs = $edit_array[0]['count'];
       if($count_logs>1)
          $count_logs = '<font color=red>'.$count_logs.'</font>';

       echo '<a href="logs.php?action=table&table=cash_book&id='.$row['id_cash_book'].'"><i class="far fa-file-alt" aria-hidden="true"></i></a>['.$count_logs.']';
       
       echo ' -'.$row['id_settlements'];
        }

   echo '</td>
   </tr>';

   }

echo '<tr>
   <td></td>
   <td></td>
   <td class=cash>'.$formatter->formatCurrency($sum_in, 'IDR').'</td>
   <td class=cash>'.$formatter->formatCurrency($sum_out, 'IDR').'</td>
   <td align=right>'.$formatter->formatCurrency($cash_position, 'IDR').'</td>
   <td></td>
   <td></td>
   <td></td>
   </tr>
   </table>
';
   }
else
    {
     echo '<p>Please choose location above</p>';
    }
?>

</div>

