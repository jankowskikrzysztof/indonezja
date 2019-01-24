

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

?>

<script>
function changePeriod(url)
{
    location.href = 'withdraw_paypal.php?period=' + url;
}
</script>


<div align=center class="pure-form" >

<input type=month value="<?php echo $period; ?>"  onchange="changePeriod(this.value)">
<br><br>
</div>


<?php


$period_year = 2018;


 $info = 'USER=contact_api1.scubafroggy.com'
        .'&PWD=K5PE5GAJFD4Z3A3F'
        .'&SIGNATURE=AFcWxV21C7fd0v3bYYYRCpSSRl31AD3Bxz.VpYPkshEEZJIKbCE.fu3s'
        .'&METHOD=TransactionSearch'
        .'&TRANSACTIONCLASS=ALL'
        .'&STARTDATE='.$period_year.'-11-01T00:00:01Z'
        .'&ENDDATE='.$period_year.'-11-30T23:59:59Z'
        .'&VERSION=94';

$curl = curl_init('https://api-3t.paypal.com/nvp');
curl_setopt($curl, CURLOPT_FAILONERROR, true);
curl_setopt($curl, CURLOPT_FOLLOWLOCATION, true);
curl_setopt($curl, CURLOPT_RETURNTRANSFER, true);
curl_setopt($curl, CURLOPT_SSL_VERIFYHOST, false);
curl_setopt($curl, CURLOPT_SSL_VERIFYPEER, false);

curl_setopt($curl, CURLOPT_POSTFIELDS,  $info);
curl_setopt($curl, CURLOPT_HEADER, 0);
curl_setopt($curl, CURLOPT_POST, 1);

$result = curl_exec($curl);
$result = explode("&", $result);
foreach($result as $value){
    $value = explode("=", $value);
    $temp[$value[0]] = $value[1];
}

for($i=0; $i<count($temp)/11; $i++){
    $returned_array[$i] = array(
        "timestamp"         =>    urldecode($temp["L_TIMESTAMP".$i]),
        "timezone"          =>    urldecode($temp["L_TIMEZONE".$i]),
        "type"              =>    urldecode($temp["L_TYPE".$i]),
        "email"             =>    urldecode($temp["L_EMAIL".$i]),
        "name"              =>    urldecode($temp["L_NAME".$i]),
        "transaction_id"    =>    urldecode($temp["L_TRANSACTIONID".$i]),
        "status"            =>    urldecode($temp["L_STATUS".$i]),
        "amt"               =>    urldecode($temp["L_AMT".$i]),
        "currency_code"     =>    urldecode($temp["L_CURRENCYCODE".$i]),
        "fee_amount"        =>    urldecode($temp["L_FEEAMT".$i]),
        "net_amount"        =>    urldecode($temp["L_NETAMT".$i]));

}



if($personel_location_selected <> '' and $action<>'edit')
$def_location = 'and location.id_location IN ('.$personel_location_selected.')';
else
$def_location = '';

if($_SESSION['location_id_limit']<>0)
      $location_limit = 'and location.id_location IN ('.$_SESSION['location_id_limit'].')';
else
      $location_limit = '';


echo '<h2 align=center>Paypal withdraw: '.$period_month.'/'.$period_year.'</h2>';


echo '<table class="pure-table pure-table-horizontal pure-table-striped">
   <thead>
<th>No</th>
<th align=center>Timestamp</th><th align=center>Transaction ID</th><th align=center>Type</th><th align=center>email</th><th align=center>Name</th><th align=center>Status</th><th align=center>Amount</th><th align=center>Cur</th>
</thead>';


foreach($returned_array as $v){
$lp++;
    echo "<tr>";
echo '<td align=center>'.$lp.'</td>';

$timez = explode('T',$v[timestamp]);

echo "<td align=center>$timez[0]<br>".str_replace('Z','',$timez[1])." $v[timezone]</td>";

echo "<td align=center>$v[transaction_id]</td>";

echo "<td align=center>$v[type]</td>";

echo "<td align=center>$v[email]</td>";

echo "<td align=left>$v[name]</td>";

echo "<td align=center>$v[status]</td>";

echo "<td align=right>$v[amt]<br>$v[fee_amount]<br>$v[net_amount]</td>";

echo "<td align=left>$v[currency_code]</td>";


echo '<td align=left>';


$account_no = 'PP: scubafroggy.com';
$transaction_id = $v[transaction_id];
$post_date = $timez[0];
$value_date = $timez[0];
$value_time = str_replace('Z','',$timez[1]);
$email = $v[email];
$type = $v[type];
$description = $v[name];
$status = $v[status];
$currency_code = $v[currency_code];
$brut_amt = $v[amt];
$fee_amount = $v[fee_amount];

$credit = 0;
$debit = 0;

if($v[net_amount]>0)
$credit = abs($v[net_amount]);
if($v[net_amount]<0)
$debit = abs($v[net_amount]);

$insert = "INSERT INTO `bank_statement`
(`account_no`,
`transaction_id`,
`post_date`,
`value_date`,
`value_time`,
`email`,
`type`,
`description`,
`brut_amt`,
`fee_amount`,
`debit`,
`credit`,
`currency_code`,
`status`
)

VALUES ('".$account_no."',
'".$transaction_id."',
'".$post_date."',
'".$value_date."',
'".$value_time."',
'".$email."',
'".$type."',
'".addslashes($description)."',
'".abs($brut_amt)."',
'".$fee_amount."',
'".$debit."',
'".$credit."',
'".$currency_code."',
'".$status."'
)
";


$select = 'SELECT transaction_id FROM `bank_statement` WHERE transaction_id="'.$transaction_id.'"';
echo $select.'<br>';
$stmt = $dbh->prepare($select);
$stmt -> execute();
$number_of_rows = $stmt->fetchColumn(); 
echo 'a:'.$number_of_rows.'<br>';
if($number_of_rows==0 and $status<>'Pending' and $v[transaction_id]<>'')
    {
        $stmt = $dbh->prepare($insert);
        $stmt -> execute();
        echo $insert;
    }


//$stmt = $dbh->prepare($insert);
//if($mysql_active==1) $stmt -> execute();

//$insert_id = $dbh->lastInsertId();  

echo "</td>";

//if($lp==3)
//exit;

}
echo "</table>";


//echo "<pre>";
//var_dump($v);
//echo "</pre>";




?>




<?php include('inc/footer.inc'); ?>
