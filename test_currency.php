<?php
$header_title = 'Scuba Froggy Panel';
include('inc/header.inc');

include('inc/config.inc');
include('inc/nav.inc');

include('inc/calc_cur.inc');
?>

<script>
$(document).ready(function(){
  $('.money5').mask('###0.00', {reverse: true});
});
</script>

<?php
$pay1 = '100000';
$pay2 = '1000.01';
$pay3 = '1000.001';


/* 
echo '<br><br>'.$pay1.'<input type="text" name="pay_bank_deposit2" id="pay_bank_deposit2" value="'.str_replace('.00','',str_replace(',','',$pay1)).'" class="money5">';
echo '<br><br>'.$pay2.'<input type="text" name="pay_bank_deposit2" id="pay_bank_deposit2" value="'.str_replace('.00','',str_replace(',','',$pay2)).'" class="money5">';
echo '<br><br>'.$pay3.'<input type="text" name="pay_bank_deposit2" id="pay_bank_deposit2" value="'.str_replace('.00','',str_replace(',','',$pay3)).'" class="money5">';
 */

echo '<input type="file" accept="image/*" capture="camera">';

phpinfo();

include('inc/footer.inc'); ?>