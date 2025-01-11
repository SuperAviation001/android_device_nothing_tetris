#!/vendor/bin/sh
if [ -f /mnt/vendor/persist/keybox/attest_keybox.so ]; then 
  setprop vendor.ontim.cit.google.key true
else
  setprop vendor.ontim.cit.google.key false
fi

if [ -f /mnt/vendor/persist/keybox/attestation_ids.so ]; then 
  setprop vendor.ontim.cit.ids true
else
  setprop vendor.ontim.cit.ids false
fi


