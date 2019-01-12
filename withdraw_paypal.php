

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





 $info = 'USER=contact_api1.scubafroggy.com'
        .'&PWD=K5PE5GAJFD4Z3A3F'
        .'&SIGNATURE=AFcWxV21C7fd0v3bYYYRCpSSRl31AD3Bxz.VpYPkshEEZJIKbCE.fu3s'
        .'&METHOD=TransactionSearch'
        .'&TRANSACTIONCLASS=ALL'
        .'&STARTDATE='.$period_year.'-'.$period_month.'-01T00:00:01Z'
        .'&ENDDATE='.$period_year.'-'.$period_month.'-31T23:59:59Z'
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


}
echo "</table>";


//echo "<pre>";
//var_dump($v);
//echo "</pre>";




?>




<?php include('inc/footer.inc'); ?>
