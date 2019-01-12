<?php

?>

<script>
function loadPage(url)
{
    location.href = url;
}
</script>



<script>
function changePeriod(url)
{
    location.href = 'budget.php?loc_sel=<?php echo $_GET['loc_sel'];?>&period=' + url;
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


setlocale(LC_MONETARY, 'id_ID');


/*

SELECT budget.id_budget, budget.date, budget.value, budget.currency,cash_book_group.name as cb_name, cash_book_group.type as cb_type, location_groups.name as loc_name
                              FROM budget,cash_book_group, location_groups
                              WHERE YEAR(budget.date)='".$period_year."'
                              and MONTH(budget.date)='".$period_month."'
                              and budget.cash_book_group_id=cash_book_group.id_cash_book_group
                              and budget.location_groups_id=location_groups.id_location_groups

                              ORDER BY loc_name, cash_book_group.name

*/


$stmt = $dbh->prepare("SELECT activity.id_activity, activity.name as act_name, activity.shortcut as act_shortcut
                              FROM activity
                              ORDER BY activity.name");
$stmt -> execute();
$activity_array = $stmt->fetchAll();



if($_GET['loc_sel']<>0)
   {
   $loc_sel = $_GET['loc_sel'];
   $where_limit = 'and location_groups.id_location_groups='.$loc_sel;
   }

$stmt = $dbh->prepare("SELECT budget.id_budget, budget.date, budget.value, budget.amount, budget.activity_id, budget.currency,cash_book_group.name as cb_name, cash_book_group.type as cb_type,
                              location_groups.name as loc_name,
                              cash_book_group.id_cash_book_group, location_groups.id_location_groups,
			      cash_book_group.id_cash_book_group
                              FROM (`location_groups`, `cash_book_group`)
                              left join budget on cash_book_group.id_cash_book_group=budget.cash_book_group_id and location_groups.id_location_groups=budget.location_groups_id
                                   and YEAR(budget.date)='".$period_year."' and MONTH(budget.date)='".$period_month."'
			      WHERE cash_book_group.only_cash=0
                              ".$where_limit."
                       ORDER BY location_groups.name, cash_book_group.name, cash_book_group.type");
$stmt -> execute();


//echo "\nPDOStatement::errorInfo():\n";
//$arr = $stmt->errorInfo();
//print_r($arr);


$row_array = $stmt->fetchAll();

$lp=0;

foreach($row_array as $row)
   {

   $loc_list[$row['id_location_groups']] = $row['loc_name'];


/*
   if($row['cb_name']=='Activity sale')
     {
     foreach($activity_array as $row_act)
        {

        $sel = "SELECT SUM(cash_book_item.value) as sum_value, COUNT(cash_book_item.value) as count_act
	                      FROM cash_book, cash_book_item, location, location_groups
	                      WHERE cash_book.cash_book_group_id = ".$row['id_cash_book_group']."
				   and cash_book.location_id=location.id_location
				   and cash_book.id_cash_book = cash_book_item.cash_book_id
				   and cash_book_item.activity_id = ".$row_act['id_activity']."
				   and location.location_groups_id=location_groups.id_location_groups
			           and location_groups.id_location_groups = ".$row['id_location_groups']."
                                   and YEAR(cash_book.date)='".$period_year."' and MONTH(cash_book.date)='".$period_month."'
	                      ";


	//echo $sel.'<br>';

	    $stmt_cb_result = $dbh->prepare($sel);
	    $stmt_cb_result -> execute();


	    $cb_result_array = $stmt_cb_result->fetchAll();
    	    $sum_cb_result = $cb_result_array[0][sum_value];
    	    $count_act = $cb_result_array[0][count_act];


             $lp++;
             echo '<tr>
                  <td align=right>'.$lp.'</td>

                  <td align=center>'.$row['loc_name'].'</td>

                  <td>'.$row['cb_name'].' - '.$row_act['act_shortcut'].': '.$row_act['act_name'].'</td>
                  <td align=center>'.$row['amount'].'</td>';

                  if($row['cb_type']==1)
                          {
                          $sum_income += $row['value11'];
                          echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td><td></td>';
                          }
                  if($row['cb_type']==2)
                          {
                          $sum_costs += $row['value11'];
                          echo '<td class=cash></td><td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';
                          }

    echo '<td align=center>'.$count_act.'</td>';

    echo '<td class=cash>'.$formatter->formatCurrency($sum_cb_result, 'IDR').'</td>';

    $sum_result += $sum_cb_result;


                  if($budget_actitivy[ $row['id_budget'] ][ $row_act['id_activity'] ][id_budget_activity]<>0)
                         $action_href = 'edit_ba&id='.$budget_actitivy[ $row['id_budget'] ][ $row_act['id_activity'] ][id_budget_activity];
                  else
                         $action_href = 'add_ba&pre_cb='.$row['id_cash_book_group'].'&pre_loc='.$row['id_location_groups'].'&pre_act='.$row_act['id_activity'];

              echo '<td><a href="?period='.$period.'&action='.$action_href.'"><i class="far fa-edit" aria-hidden="true"></i></a></td>';

              echo '</tr>';
        }
     }
   else
     {
*/

    $sel = "SELECT SUM(value) as sum_value
	                      FROM cash_book, location, location_groups
	                      WHERE cash_book.cash_book_group_id = ".$row['id_cash_book_group']."
				   and cash_book.location_id=location.id_location
				   and location.location_groups_id=location_groups.id_location_groups
			           and location_groups.id_location_groups = ".$row['id_location_groups']."
                                   and YEAR(cash_book.date)='".$period_year."' and MONTH(cash_book.date)='".$period_month."'
	                      ";

    //echo $sel.'<br>';

    $stmt_cb_result = $dbh->prepare($sel);
    $stmt_cb_result -> execute();

//echo "\nPDOStatement::errorInfo():\n";
//$arr = $stmt->errorInfo();
//print_r($arr);


    $cb_result_array = $stmt_cb_result->fetchAll();
        $sum_cb_result = $cb_result_array[0][sum_value];





    $lp++;
     echo '<tr>
     <td align=right>'.$lp.'</td>

     <td align=center>'.$row['loc_name'].'</td>

     <td>'.$row['cb_name'].'</td>
     <!--<td></td>-->';

     if($row['cb_type']==1)
        {
        $sum_income += $row['value'];
        echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';
        }
     if($row['cb_type']==2)
        {
        $sum_costs += $row['value'];
        echo '<td class=cash>'.$formatter->formatCurrency($row['value'], 'IDR').'</td>';
        }

    //echo '<td></td>';
    echo '<td class=cash>'.$formatter->formatCurrency($sum_cb_result, 'IDR').'</td>';

    $sum_result += $sum_cb_result;


     if($row['id_budget']<>0)
       $action_href = 'edit&id='.$row['id_budget'];
     else
       $action_href = 'add&pre_cb='.$row['id_cash_book_group'].'&pre_loc='.$row['id_location_groups'];

     echo '<td><a href="?period='.$period.'&action='.$action_href.'"><i class="far fa-edit" aria-hidden="true"></i></a></td>';

     echo '</tr>';

//     }
  }


?>
</tbody>
   <thead>
<!--  <tr>
      <th colspan=7><a class="pure-button pure-button-primary" href="?period=<?php echo $period; ?>&action=add"><i class="fa fa-plus" aria-hidden="true"></i> Add</a></th>
  </tr>-->
  <tr align=center valign=top>
     <th>No.</th>
     <th>Group<br>

              <div class="pure-form"><select onchange="loadPage(this.value)">
                      <option value='?'>All</option>
     <?php
          foreach ($loc_list as $loc_idx => $loc_name) {


              if($loc_idx == $loc_sel)
                 $sel = 'selected';
              else
                 $sel = '';

              echo '<option value="?loc_sel='.$loc_idx.'" '.$sel.'>'.$loc_name.'</option>';

              }

     ?>

              </select></div></th>
     <th>Group Name</th>
<!--     <th>Budget Amount</th> -->
     <th>Budget</th>
<!--     <th>Result Amount</th> -->
     <th>Result Cash</th>
     <th></th>
  </tr>
 </thead>
<!--
<tfoot>
    <tr>
      <td colspan=4 align=right>Sum</td>
      <td></td>
      <td></td>
      <td></td>
      <td></td>
     <?php echo '<td class=cash>'.$formatter->formatCurrency($sum_result, 'IDR').'</td>'; ?>
      <td></td>
    </tr>
</tfoot>
-->
</table>
</div>
