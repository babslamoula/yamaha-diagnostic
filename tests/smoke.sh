#!/usr/bin/env bash
# tests/smoke.sh - reproduit les 3 bugs P0 et valide qu'ils sont fixes.
#
# Bug #1 : EOF stdin -> le binaire devait bouclait, doit maintenant exit < 5s.
# Bug #2 : cycle A -> B ; B -> A ; -> segfault avant, doit retourner "pas
#          verifie" sans crasher.
# Bug #3 : MT-07 + cliquetis -> doit remonter "Maladie CCT" CP2 (200 EUR)
#          et non plus "Tendeur chaine distrib" generique (150 EUR).

set -u
BIN="./yamaha_diag"
PASS=0
FAIL=0
FAILED_NAMES=()

run_with_timeout() {
    # $1 = timeout seconds, $2 = file with stdin, $3 = output file
    local secs=$1 inp=$2 out=$3
    ( "$BIN" < "$inp" > "$out" 2>&1 ) &
    local pid=$!
    local i=0
    while kill -0 "$pid" 2>/dev/null && [ "$i" -lt "$secs" ]; do
        sleep 1
        i=$((i + 1))
    done
    if kill -0 "$pid" 2>/dev/null; then
        kill -9 "$pid" 2>/dev/null
        return 124
    fi
    wait "$pid" 2>/dev/null
    return $?
}

check() {
    local name=$1 status=$2
    if [ "$status" = "PASS" ]; then
        printf "  [PASS] %s\n" "$name"
        PASS=$((PASS + 1))
    else
        printf "  [FAIL] %s\n" "$name"
        FAIL=$((FAIL + 1))
        FAILED_NAMES+=("$name")
    fi
}

[ -x "$BIN" ] || { echo "Binaire $BIN absent. Lance 'make' d'abord."; exit 1; }

echo "==== Tests fumee ===="

# --- Bug #1 : EOF stdin ---
echo "1" > /tmp/yd_t1.in
run_with_timeout 5 /tmp/yd_t1.in /tmp/yd_t1.out
if [ $? -eq 124 ]; then
    check "Bug #1 - EOF stdin ne hang plus" FAIL
else
    check "Bug #1 - EOF stdin ne hang plus" PASS
fi

# --- Bug #2 : cycle ---
cat > /tmp/yd_t2_rules.txt <<'EOF'
A -> B ;
B -> A ;
EOF
cat > /tmp/yd_t2.in <<EOF

5
0
/tmp/yd_t2_rules.txt

3
A
non
non

0
EOF
run_with_timeout 5 /tmp/yd_t2.in /tmp/yd_t2.out
rc=$?
if [ "$rc" -eq 124 ]; then
    check "Bug #2 - cycle ne hang plus" FAIL
elif [ "$rc" -gt 128 ] || [ "$rc" -eq 139 ]; then
    check "Bug #2 - cycle ne segfault plus" FAIL
elif grep -q "Resultat : A ne peut pas etre verifie" /tmp/yd_t2.out; then
    check "Bug #2 - cycle correctement rejete" PASS
else
    check "Bug #2 - cycle correctement rejete" FAIL
fi

# --- Bug #3 : MT-07 + cliquetis -> CP2 specifique ---
{
    echo ""
    echo "1"
    echo "3"
    echo "1"
    for i in $(seq 1 50); do echo "oui"; done
    echo ""
    echo "0"
} > /tmp/yd_t3.in
run_with_timeout 8 /tmp/yd_t3.in /tmp/yd_t3.out
if grep -q "Maladie CCT MT-07" /tmp/yd_t3.out; then
    check "Bug #3 - diagnostic CP2 specifique l'emporte" PASS
elif grep -q "Tendeur Chaine Distri" /tmp/yd_t3.out; then
    check "Bug #3 - diagnostic CP2 specifique l'emporte (regression: generique gagne)" FAIL
else
    check "Bug #3 - diagnostic CP2 specifique l'emporte (aucun diag remonte)" FAIL
fi

echo
echo "==== Resultat : $PASS pass / $FAIL fail ===="
if [ "$FAIL" -ne 0 ]; then
    printf "  Echecs : %s\n" "${FAILED_NAMES[*]}"
    exit 1
fi
exit 0
