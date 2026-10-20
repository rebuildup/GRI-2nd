#!/usr/bin/env bash
set -Eeuo pipefail

# V2仕様書関連ファイルを更新するスクリプト
# 実行場所: 2ndディレクトリ直下
# 方針:
#   1. V2編集元が存在しない場合だけ、対応するV1編集元をコピーする
#   2. 既存のV2編集元は上書きしない
#   3. 出力がない、または編集元の方が新しい場合だけ再生成する
#   4. 最後にmakeを実行し、仕様書PDFを更新する

ROOT_DIR=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
cd "$ROOT_DIR"

FORCE=0
SKIP_MAKE=0

usage() {
    cat <<'EOF'
使い方: ./update_v2.sh [オプション]

オプション:
  -f, --force       生成済みファイルも強制的に再生成する
  --skip-make       最後のmakeを実行しない
  -h, --help        このヘルプを表示する
EOF
}

while (($# > 0)); do
    case "$1" in
        -f|--force) FORCE=1 ;;
        --skip-make) SKIP_MAKE=1 ;;
        -h|--help) usage; exit 0 ;;
        *)
            printf '不明なオプション: %s\n' "$1" >&2
            usage >&2
            exit 2
            ;;
    esac
    shift
done

errors=0
updated=0
skipped=0

info() { printf '[INFO] %s\n' "$*"; }
ok() { printf '[OK]   %s\n' "$*"; }
warn() { printf '[WARN] %s\n' "$*" >&2; }
err() { printf '[ERROR] %s\n' "$*" >&2; errors=$((errors + 1)); }

have_cmd() {
    command -v "$1" >/dev/null 2>&1
}

copy_if_missing() {
    local src=$1
    local dst=$2

    if [[ -e "$dst" ]]; then
        info "既存の編集元を保持: $dst"
        skipped=$((skipped + 1))
        return 0
    fi

    if [[ ! -f "$src" ]]; then
        err "コピー元がありません: $src"
        return 1
    fi

    mkdir -p "$(dirname -- "$dst")"
    cp -p -- "$src" "$dst"
    ok "V1からV2を作成: $src -> $dst"
    updated=$((updated + 1))
}

needs_update() {
    local src=$1
    local dst=$2
    [[ $FORCE -eq 1 || ! -s "$dst" || "$src" -nt "$dst" ]]
}

generate_plantuml() {
    local src=$1
    local dst=${src%.pu}.png

    if [[ ! -f "$src" ]]; then
        err "PlantUML編集元がありません: $src"
        return 1
    fi

    if ! needs_update "$src" "$dst"; then
        info "最新: $dst"
        skipped=$((skipped + 1))
        return 0
    fi

    if ! have_cmd plantuml; then
        err "plantumlコマンドがありません。生成できません: $dst"
        return 1
    fi

    info "PlantUMLをPNG化: $src"
    if plantuml -tpng -- "$src"; then
        if [[ -s "$dst" ]]; then
            ok "生成: $dst"
            updated=$((updated + 1))
        else
            err "PlantUMLは終了しましたが出力がありません: $dst"
            return 1
        fi
    else
        err "PlantUMLの生成に失敗: $src"
        return 1
    fi
}

generate_pad_pdf() {
    local src=$1
    local dst=$2
    local tmp

    if [[ ! -f "$src" ]]; then
        err "PAD編集元がありません: $src"
        return 1
    fi

    if ! needs_update "$src" "$dst"; then
        info "最新: $dst"
        skipped=$((skipped + 1))
        return 0
    fi

    if ! have_cmd pad2ps; then
        err "pad2psコマンドがありません。生成できません: $dst"
        return 1
    fi
    if ! have_cmd ps2pdf; then
        err "ps2pdfコマンドがありません。生成できません: $dst"
        return 1
    fi

    mkdir -p "$(dirname -- "$dst")"
    tmp=$(mktemp "${TMPDIR:-/tmp}/pad-v2.XXXXXX.ps")

    info "PADをPDF化: $src"
    if pad2ps -c "$src" > "$tmp" && [[ -s "$tmp" ]] && ps2pdf "$tmp" "$dst"; then
        if [[ -s "$dst" ]]; then
            ok "生成: $dst"
            updated=$((updated + 1))
        else
            err "ps2pdfは終了しましたが出力がありません: $dst"
            rm -f -- "$tmp"
            return 1
        fi
    else
        err "PAD PDFの生成に失敗: $src"
        rm -f -- "$tmp" "$dst"
        return 1
    fi

    rm -f -- "$tmp"
}

run_doxygen() {
    local src_dir=''

    for candidate in srcV2 'srcV２'; do
        if [[ -d "$candidate" && -f "$candidate/Doxyfile" ]]; then
            src_dir=$candidate
            break
        fi
    done

    if [[ -z "$src_dir" ]]; then
        warn "srcV2/srcV２のDoxyfileがないため、Doxygenは省略します"
        return 0
    fi

    if ! have_cmd doxygen; then
        warn "doxygenコマンドがないため、Doxygenは省略します"
        return 0
    fi

    info "Doxygenを更新: $src_dir"
    if (cd "$src_dir" && doxygen Doxyfile > doxygen.log 2>&1); then
        if [[ -f "$src_dir/html/index.html" ]]; then
            ok "Doxygen生成: $src_dir/html/index.html"
            updated=$((updated + 1))
        else
            warn "Doxygenは終了しましたがhtml/index.htmlがありません。$src_dir/doxygen.logを確認してください"
        fi
    else
        err "Doxygen生成失敗。ログ: $src_dir/doxygen.log"
        return 1
    fi
}

run_make() {
    if [[ $SKIP_MAKE -eq 1 ]]; then
        info "--skip-makeによりmakeを省略します"
        return 0
    fi

    if [[ ! -f Makefile && ! -f makefile ]]; then
        warn "Makefileがないため、仕様書PDF生成は省略します"
        return 0
    fi

    if ! have_cmd make; then
        err "makeコマンドがありません"
        return 1
    fi

    info "仕様書をmakeで更新"
    if make 2>&1 | tee make-v2.log; then
        ok "make完了。ログ: make-v2.log"
        updated=$((updated + 1))
    else
        err "make失敗。ログ: make-v2.log"
        return 1
    fi
}

info "作業ディレクトリ: $ROOT_DIR"

# V2編集元がなければV1から作成する。既存V2は絶対に上書きしない。
copy_if_missing 'Usecase/usecaseV1.pu' 'Usecase/usecaseV2.pu' || true
copy_if_missing 'SequenceDiagram/sequenceV1.pu' 'SequenceDiagram/sequenceV2.pu' || true
copy_if_missing 'AssociationChart/GaiyouModuleV1.pu' 'AssociationChart/GaiyouModuleV2.pu' || true
copy_if_missing 'AssociationChart/DetailModuleV1.pu' 'AssociationChart/DetailModuleV2.pu' || true
copy_if_missing 'PAD/GaiyouPadV1.c' 'PAD/GaiyouPadV2.c' || true
copy_if_missing 'PAD/DetailPadV1.c' 'PAD/DetailPadV2.c' || true

# specV2.mdがない場合だけ、候補ファイルから作成する。
if [[ ! -f specV2.md ]]; then
    if [[ -f spevV2.md ]]; then
        cp -p -- spevV2.md specV2.md
        ok 'spevV2.mdからspecV2.mdを作成'
        updated=$((updated + 1))
    elif [[ -f specV1.md ]]; then
        cp -p -- specV1.md specV2.md
        ok 'specV1.mdからspecV2.mdを作成'
        warn 'specV2.mdのタイトル、バージョン、本文をV2仕様へ修正してください'
        updated=$((updated + 1))
    else
        warn 'specV2.mdがなく、コピー可能なspevV2.md/specV1.mdもありません'
    fi
else
    info '既存の編集元を保持: specV2.md'
fi

# PlantUML図を更新する。
generate_plantuml 'Usecase/usecaseV2.pu' || true
generate_plantuml 'SequenceDiagram/sequenceV2.pu' || true
generate_plantuml 'AssociationChart/GaiyouModuleV2.pu' || true
generate_plantuml 'AssociationChart/DetailModuleV2.pu' || true

# PAD図を更新する。
generate_pad_pdf 'PAD/GaiyouPadV2.c' 'PAD/GaiyouPadV2PAD.pdf' || true
generate_pad_pdf 'PAD/DetailPadV2.c' 'PAD/DetailPadV2PAD.pdf' || true

# ソース文書と仕様書PDFを更新する。
run_doxygen || true
run_make || true

printf '\n===== 更新結果 =====\n'
printf '更新・生成: %d\n' "$updated"
printf '変更なし:   %d\n' "$skipped"
printf 'エラー:     %d\n' "$errors"

printf '\nV2関連ファイル:\n'
find Usecase SequenceDiagram AssociationChart PAD -maxdepth 1 -type f \
    \( -name '*V2*' -o -name 'sequenceV1.png' \) -print 2>/dev/null | sort || true
[[ -f specV2.md ]] && printf '%s\n' 'specV2.md'
find . -maxdepth 1 -type f -name '*V2*.pdf' -print 2>/dev/null | sort || true

if [[ $errors -ne 0 ]]; then
    printf '\n一部の処理に失敗しました。上記の[ERROR]とログを確認してください。\n' >&2
    exit 1
fi

printf '\nすべての更新処理が完了しました。\n'
