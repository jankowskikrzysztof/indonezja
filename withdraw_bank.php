<style>
         tr {
             font-size: 85%;
         }
      </style>

<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

$period = $_GET['period'];
if(!$period)
  $period = date('Y').'-'.date('m');

$period_month = substr($period,5,2);
$period_year = substr($period,0,4);

$account_set = $_GET['account'];
if(!$account_set)
  $account_set = '';

if($account_set<>'')
    $account_where = 'and bank_statement.account_no='.$account_set;
else   
    $account_where = '';


$type_set = $_GET['type_set'];
    if(!$type_set)
      $type_set_sql = '';
    
    if($type_set<>'')
        $type_set_sql = 'and bank_statement.type LIKE "'.$type_set.'%"';
    else   
        $type_set_sql = '';
    

?>

<script>
function changePeriod(url)
{
    location.href = 'withdraw_bank.php?account=<?php echo $account_set;?>&type_set=<?php echo $type_set;?>&period=' + url;
}
</script>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>


<?php



echo '<h2 align=center>Bank withdraw: '.$period_month.'/'.$period_year.'</h2>';


echo '<table class="pure-table pure-table-horizontal pure-table-striped">';


if($_SESSION['user_id']==1 or $_SESSION['user_id']==6 )
echo '<thead>
<th colspan=8 align=right><a href="import_bank.php">Import withdraw</a></th>
<th colspan=2 align=right><a href="auto_settlements.php">Auto Settlements</a></th>
</thead>';

echo '<thead>
<th>No</th>
<th align=center>Account No</th>
<th align=center>Post Date/<br>Value Date</th>
<th align=center>Branch/<br>Journal</th>
<th align=center>Type</th>
<th align=center>Description</th>
<th align=center>Credit<br>IN</th>
<th align=center>Debit<br>OUT</th>
<th align=center>Settlements</th>
<th align=center>Set. Value</th>
</thead>';
//
$select = "SELECT bank_statement.*, GROUP_CONCAT(settlements.id_settlements) as settlements, sum(settlements.value) as set_value
FROM bank_statement
left join settlements on settlements.bank_statement_id=bank_statement.id_bank_statement
WHERE YEAR(bank_statement.post_date)='".$period_year."'
and MONTH(bank_statement.post_date)='".$period_month."'
".$account_where."
".$type_set_sql."
GROUP BY bank_statement.id_bank_statement
ORDER BY bank_statement.post_date";

//echo $select;

$stmt = $dbh->prepare($select);
$stmt -> execute();

$row_array = $stmt->fetchAll();


foreach($row_array as $row)
   {
$lp++;
    echo "<tr>";

if(!$row['settlements'])
    $set_bgcolor = 'style="background-color: #ff9999"';
else
    $set_bgcolor = '';

if($row['credit']+$row['debit'] <> $row['set_value'])
    $setvalue_bgcolor = 'style="background-color: #ff9999"';
else
    $setvalue_bgcolor = '';


echo '<td align=center>'.$lp.'<br><font size="0.5vw">'.$row['id_bank_statement'].'</font></td>';

echo "<td align=center><a href='?account=\"".$row['account_no']."\"&period=".$period."'>".$row['account_no']."</a></td>";
echo "<td align=center>".$row['post_date']."<br>".$row['value_date']."</td>";
echo "<td align=center>".$row['branch']."<br>".$row['journal_no']."</td>";
echo "<td align=center>".$row['type']."</td>";
echo "<td align=left>".$row['description']."</td>";
echo "<td align=right>".$formatter->formatCurrency($row['credit'], 'IDR')."</td>";
echo "<td align=right>".$formatter->formatCurrency($row['debit'], 'IDR')."</td>";
echo "<td align=right ".$set_bgcolor.">".$row['settlements']."</td>";
echo "<td align=right ".$setvalue_bgcolor.">".$formatter->formatCurrency($row['set_value'], 'IDR')."</td>";

$debit_sum += $row['debit'];
$credit_sum += $row['credit'];


}

echo '<tr><td colspan=6 align=right>SUM</td><td align="right">'.$formatter->formatCurrency($credit_sum, 'IDR').'</td><td align="right">'.$formatter->formatCurrency($debit_sum, 'IDR').'</td><td></td><td></td></tr>';

echo "</table>";

?>




<?php include('inc/footer.inc'); ?>